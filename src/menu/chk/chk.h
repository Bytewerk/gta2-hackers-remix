#pragma once

typedef struct {
  char *settings_path;
  char *fullpath;
  char *latest_version;
  char is_version_set;
  char is_running;
} chk_t;

chk_t *chk_init(char *settings_path);
void chk_cleanup(chk_t *chk);

#define G2HR_CHK_BUFFER_MAXLEN 20
#define G2HR_CHK_SETTINGS_FILE "updatecheck.ini"
