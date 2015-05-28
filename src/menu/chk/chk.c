#include "chk.h"
#include "../../common/cfg/cfg.h"
#include <SDL2/SDL_net.h>

// if the connection fails, this thread quits silently
int chk_thread(void *userdata) {
  chk_t *chk = (chk_t *)userdata;

  // TODO: load settings and read the last update check date

  // resulve the hostname, open the socket
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, "localhost", 80) < 0) {
    chk->is_running = 0;
    return -1;
  }
  TCPsocket sock = SDLNet_TCP_Open(&ip);
  if (!sock) {
    SDLNet_TCP_Close(sock);
    chk->is_running = 0;
    return -2;
  }

  // request the version, write it into a buffer
  // (expected format: version\n1.2.3.4\n)
  char *get = "GET /v\n";
  SDLNet_TCP_Send(sock, get, strlen(get) + 1);
  char *buffer = malloc(CHK_BUFFER_MAXLEN + 1);
  SDLNet_TCP_Recv(sock, buffer, CHK_BUFFER_MAXLEN);
  buffer[CHK_BUFFER_MAXLEN] = '\0';

  // replace all new lines with '\0'
  for (int i = 0; i < CHK_BUFFER_MAXLEN; i++)
    if (buffer[i] == '\n')
      buffer[i] = '\0';

  // the first line should be "version", fail otherwise
  if (strcmp(buffer, "version")) {
    free(buffer);
    SDLNet_TCP_Close(sock);
    chk->is_running = 0;
    return -3;
  }

  // save the actual version number (second line) in the chk struct
  // and free the buffer
  char *version = buffer + strlen("version") + 1;
  uint16_t version_len = strlen(version);
  chk->latest_version = malloc(version_len + 1);
  strncpy(chk->latest_version, version, version_len + 1);
  chk->latest_version[version_len] = '\0';
  free(buffer);

  // mark it as set, clean up the socket
  chk->is_version_set = 1;
  chk->is_running = 0;
  SDLNet_TCP_Close(sock);
  return 0;
}

chk_t *chk_init(char *settings_path) {
  chk_t *chk = calloc(1, sizeof(chk_t));
  chk->is_running = 1;
  SDL_DetachThread(SDL_CreateThread(&chk_thread, "updatecheck", (void *)chk));

  return chk;
}

void chk_cleanup(chk_t *chk) {
  if (chk->latest_version)
    free(chk->latest_version);
  free(chk);
}
