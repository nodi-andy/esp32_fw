#include "Config.h"
#include "SFSFile.h"

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
    grbl_sendf(CLIENT_ALL, "[closeFile:]\r\n");
    return true;
}

/*
  read a line from the SD card
  strip whitespace
  strip comments per http://linuxcnc.org/docs/ja/html/gcode/overview.html#gcode:comments
  make uppercase
  return true if a line is
*/
boolean readSFSFileLine(char* line, int maxlen) {
    if (!mySFSFile) {
        report_status_message(Error::FsFailedRead, SFS_client);
        return false;
    }
    sfs_current_line_number += 1;
    int len = 0;
    while (mySFSFile.available()) {
        if (len >= maxlen) {
            return false;
        }
        char c = mySFSFile.read();
        if (c == '\n') {
            break;
        }
        line[len++] = c;
    }
    line[len] = '\0';
    return len || mySFSFile.available();
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
