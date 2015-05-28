#pragma once

typedef struct {
  char *settings_path;
  char *latest_version;
  char is_version_set;
} chk_t;

chk_t *chk_init(char *settings_path);
void chk_cleanup(chk_t *chk);

#define CHK_BUFFER_MAXLEN 20
