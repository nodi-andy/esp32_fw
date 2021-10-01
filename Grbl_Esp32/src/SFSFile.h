#pragma once

#include "Grbl.h"
#include <FS.h>
#include <SD.h>


extern bool                       SFS_ready_next;  // Grbl has processed a line and is waiting for another
extern uint8_t                    SFS_client;
extern WebUI::AuthenticationLevel SFS_auth_level;

SDState  get_sfs_state(bool refresh);
SDState  set_sfs_state(SDState state);
boolean  openSFSFile(fs::FS& fs, const char* path);
boolean  closeSFSFile();
boolean  readSFSFileLine(char* line, int len);
void     readSFSFile(fs::FS& fs, const char* path);
uint32_t sfs_get_current_line_number();
void     sfs_get_current_filename(char* name);
