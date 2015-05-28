#include "chk.h"
#include "../../common/cfg/cfg.h"
#include <SDL2/SDL_net.h>

// if the connection fails, this thread quits silently
int chk_thread(void *userdata) {
  chk_t *chk = (chk_t *)userdata;

  // TODO: load settings and read the last update check date

  // resulve the IP
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, "localhost", 80) < 0)
    return -1;

  // open the socket
  TCPsocket sock = SDLNet_TCP_Open(&ip);
  if (!sock)
    return -1;

  // request the version
  char *get = "GET /v\n";
  SDLNet_TCP_Send(sock, get, strlen(get) + 1);

  // save the version number
  chk->latest_version = malloc(CHK_BUFFER_MAXLEN + 1);
  SDLNet_TCP_Recv(sock, chk->latest_version, CHK_BUFFER_MAXLEN);
  chk->latest_version[CHK_BUFFER_MAXLEN] = '\0';

  // replace all new lines with '\0'
  for (int i = 0; i < CHK_BUFFER_MAXLEN; i++)
    if (chk->latest_version[i] == '\n')
      chk->latest_version[i] = '\0';

  // mark it as set, clean up the socket
  chk->is_version_set = 1;
  SDLNet_TCP_Close(sock);
  return 0;
}

chk_t *chk_init(char *settings_path) {
  chk_t *chk = calloc(1, sizeof(chk_t));
  SDL_DetachThread(SDL_CreateThread(&chk_thread, "updatecheck", (void *)chk));

  return chk;
}

void chk_cleanup(chk_t *chk) {
  if (chk->latest_version)
    free(chk->latest_version);
  free(chk);
}
