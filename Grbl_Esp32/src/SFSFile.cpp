#include "Config.h"
#include "SFSFile.h"
#include <ctype.h>
// Toggle this on/off as needed
#ifndef DEBUG_SFS_LABELS
#define DEBUG_SFS_LABELS 1
#endif

#if DEBUG_SFS_LABELS
  #define SFS_DBG(fmt, ...) grbl_msg_sendf(SFS_client, MsgLevel::Info, "[SFS] " fmt, ##__VA_ARGS__)
#else
  #define SFS_DBG(fmt, ...) do {} while(0)
#endif

File                       mySFSFile;
bool                       SFS_ready_next = false;  // Grbl has processed a line and is waiting for another
uint8_t                    SFS_client     = CLIENT_SERIAL;
WebUI::AuthenticationLevel SFS_auth_level = WebUI::AuthenticationLevel::LEVEL_GUEST;
uint32_t                   sfs_current_line_number;     // stores the most recent line number read from the SD
static char                comment[LINE_BUFFER_SIZE];  // Line to be executed. Zero-terminated.

boolean openSFSFile(fs::FS& fs, const char* path) {
    mySFSFile = fs.open(path);
    if (!mySFSFile) {
        //report_status_message(Error::FsFailedRead, CLIENT_SERIAL);
        return false;
    }
    SFS_ready_next          = false;  // this will get set to true when Grbl issues "ok" message
    sfs_current_line_number = 0;
    set_sfs_state(SDState::BusyPrinting);
    return true;
}

boolean closeSFSFile() {
    if (!mySFSFile) {
        return false;
    }
    
    SFS_ready_next          = false;
    sfs_current_line_number = 0;
    mySFSFile.close();
    set_sfs_state(SDState::Idle);
    //grbl_sendf(CLIENT_ALL, "[closeFile:]\r\n");
    return true;
}

// Trim just for checking; we don't modify the returned line buffer
static inline char* ltrim(char* p) {
  while (*p && isspace((unsigned char)*p)) ++p;
  return p;
}

// Read one physical line (CRLF/LF normalized). Returns:
//  - length (>=0) if a line was read (may be 0 for blank line)
//  - -1 on EOF with nothing read
static int sfs_read_physical_line(char* out, int maxlen) {
  int len = 0;
  bool gotAny = false;
  while (mySFSFile.available()) {
    char c = mySFSFile.read();
    gotAny = true;
    if (c == '\r') continue;        // normalize CRLF
    if (c == '\n') break;           // end of line
    if (len < maxlen - 1) out[len++] = c; // truncate if necessary
  }
  out[len] = '\0';
  return (gotAny ? len : -1);
}

// Public: read next non-empty, non-label line
// Returns true if a content line was written to `line`, false on EOF/error.
// Label lines start with ':' (after optional whitespace) and are skipped.
boolean readSFSFileLine(char* line, int maxlen) {
  if (!mySFSFile) {
    report_status_message(Error::FsFailedRead, SFS_client);
    SFS_DBG("ERR: mySFSFile is null");
    return false;
  }

  const uint32_t MAX_SKIPS = 4096;   // hard cap to avoid pathological cases
  uint32_t skipped = 0;

  SFS_DBG("begin: avail=%d, line#=%lu", (int)mySFSFile.available(), (unsigned long)sfs_current_line_number);

  while (mySFSFile.available()) {
    int len = sfs_read_physical_line(line, maxlen);
    if (len < 0) {
      SFS_DBG("EOF (no chars read)");
      return false;  // EOF
    }

    sfs_current_line_number += 1;

    // Show the raw physical line (trimmed to something safe to print)
    // Limit to 80 chars to keep logs readable.
    if (len > 80) { line[80] = '\0'; }
    SFS_DBG("read L%lu len=%d: '%s'", (unsigned long)sfs_current_line_number, len, line);

    char* p = ltrim(line);

    // Empty line?
    if (*p == '\0') {
      skipped++;
      SFS_DBG("skip empty (skipped=%lu), avail=%d", (unsigned long)skipped, (int)mySFSFile.available());
      if (skipped >= MAX_SKIPS) {
        SFS_DBG("ABORT: MAX_SKIPS reached");
        return false;
      }
      continue;
    }

    // Label line?
    if (*p == ':') {
      // (Optionally print the label token)
      const char* lab = p + 1;
      SFS_DBG("skip label '%s' (L%lu), avail=%d", lab, (unsigned long)sfs_current_line_number, (int)mySFSFile.available());
      skipped++;
      if (skipped >= MAX_SKIPS) {
        SFS_DBG("ABORT: MAX_SKIPS reached");
        return false;
      }
      continue;
    }

    // Normal content line -> emit
    SFS_DBG("emit L%lu: '%s'", (unsigned long)sfs_current_line_number, p);
    return true;
  }

  SFS_DBG("done: avail=0 at L%lu", (unsigned long)sfs_current_line_number);
  return false; // no more data
}

uint32_t sfs_get_current_line_number() {
    return sfs_current_line_number;
}

SDState sfs_state = SDState::Idle;

SDState get_sfs_state(bool refresh) {
    return sfs_state;
}

SDState set_sfs_state(SDState state) {
    sfs_state = state;
    return sfs_state;
}

void sfs_get_current_filename(char* name) {
    if (mySFSFile) {
        strcpy(name, mySFSFile.name());
    } else {
        name[0] = 0;
    }
}

// SFSFile.cpp (add at file bottom, after existing functions)

bool sfs_jump_to_label(const char* label) {
    if (!mySFSFile) return false;

    // Rewind and scan
    mySFSFile.seek(0, SeekSet);
    sfs_current_line_number = 0;

    const int MAX = 256;
    char buf[MAX];

    auto ltrim = [](char* p)->char* { while (*p && isspace((unsigned char)*p)) ++p; return p; };

    while (mySFSFile.available()) {
        // read one line
        int len = 0;
        while (mySFSFile.available()) {
            if (len >= (MAX - 1)) break;
            char c = mySFSFile.read();
            if (c == '\r') continue;
            if (c == '\n') break;
            buf[len++] = c;
        }
        buf[len] = '\0';
        sfs_current_line_number += 1;

        // check for ":label"
        char* p = ltrim(buf);
        if (*p == ':') {
            ++p;                       // skip ':'
            while (*p == ' ' || *p == '\t') ++p;  // allow ":  label"
            // exact (case-sensitive) match; change to strcasecmp if you prefer case-insensitive
            if (strcmp(p, label) == 0) {
                // We're positioned just after the label line. Next call to readSFSFileLine()
                // will return the *following* line.
                return true;
            }
        }
    }
    return false; // not found
}
