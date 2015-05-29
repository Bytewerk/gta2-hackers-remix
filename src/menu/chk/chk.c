#include "chk.h"
#include "../../common/cstr/cstr.h"
#include <stdio.h>
#include <time.h>

// returns silently on failure
int chk_thread(void *userdata) {
  chk_t *chk = (chk_t *)userdata;

  // only do the check when the interval was reached
  time_t old = 0;
  time_t new = time(NULL);
  char *old_str = ini_read(chk->ini, "updatecheck", "last_check_timestamp");
  if (old_str)
    sscanf(old_str, "%li", &old);
  if (new - old < G2HR_CHK_INTERVAL)
    return 0;

  // prepare the socket
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, "localhost", 80) < 0)
    return 1;
  chk->sock = SDLNet_TCP_Open(&ip);
  if (!chk->sock)
    return 2;

  // request the version (expected format: version\n1.2.3.4\n)
  // and replace new lines with '\0'
  char *get = "GET /v\n";
  SDLNet_TCP_Send(chk->sock, get, strlen(get) + 1);
  if (SDLNet_TCP_Recv(chk->sock, chk->net_buffer, G2HR_CHK_NET_BUFFER_SIZE) < 1)
    return 3;
  for (int i = 0; i < G2HR_CHK_NET_BUFFER_SIZE; i++)
    if (chk->net_buffer[i] == '\n')
      chk->net_buffer[i] = '\0';

  // verify the "version" line, get a pointer to the real version
  if (strcmp(chk->net_buffer, "version"))
    return 4;
  char *version = chk->net_buffer + strlen("version") + 1;

  // ini: save the current timestamp
  char *new_str = malloc(50);
  snprintf(new_str, 50, "%li", new);
  ini_modify(chk->ini, "updatecheck", "last_check_timestamp", new_str, 1);
  free(new_str);

  // write the ini file
  chk->version_readable = false;
  chk->latest_version =
      ini_modify(chk->ini, "updatecheck", "latest_version", version, true);
  chk->version_readable = true;
  ini_save(chk->ini, chk->fullpath, false, false);
  return 0;
}

chk_t *chk_init(char *settings_path_with_trailing_slash, bool in_background) {
  chk_t *chk = calloc(1, sizeof(chk_t));
  chk->fullpath =
      cstr_merge(settings_path_with_trailing_slash, G2HR_CHK_SETTINGS_FILE);
  chk->ini = ini_open(chk->fullpath, 1);
  chk->latest_version = ini_read(chk->ini, "updatecheck", "latest_version");
  chk->version_readable = true;

  if (in_background)
    chk->thread = SDL_CreateThread(&chk_thread, "updatecheck", (void *)chk);
  else
    chk_thread((void *)chk);

  return chk;
}

char *chk_latest_version(chk_t *chk) {
  return chk->version_readable ? chk->latest_version : NULL;
}

void chk_cleanup(chk_t *chk) {
  SDLNet_TCP_Close(chk->sock);
  SDL_WaitThread(chk->thread, NULL);
  free(chk->fullpath);
  ini_cleanup(chk->ini);
  free(chk);
}
