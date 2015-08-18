#pragma once
#include "../../common/ini/ini.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>

#define G2HR_CHK_NET_BUFFER_SIZE 500
#define G2HR_CHK_SETTINGS_FILE "updatecheck.ini"
#define G2HR_CHK_INTERVAL (3600 * 24) // one day
#define G2HR_CHK_HOST "g2hr.org"
#define G2HR_CHK_VERSION_MAXLEN 7

typedef struct {
  ini_t *ini;

  char *latest_version;
  bool version_readable;

  char net_buffer[G2HR_CHK_NET_BUFFER_SIZE + 1];
  TCPsocket sock;
  SDL_Thread *thread;

} chk_t;

chk_t *chk_init(char *settings_path_with_trailing_slash, bool in_background);
char *chk_latest_version(chk_t *chk);
void chk_cleanup(chk_t *chk);
