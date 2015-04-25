#include "server.h"
#include "menu_meta_protocol.h"
#include <SDL2/SDL_net.h>

// listen on all addresses. Better would be localhost only, but SDL2_net
// doesn't support this. We'll check for localhost when establishing
// the connection though.
server_t *server_init() {
  server_t *server = malloc(sizeof(server_t));
  server->sock = 0;

  SDLNet_ResolveHost(&(server->ip), NULL, G2HR_MENU_SERVER_PORT);
  server->sock_server = SDLNet_TCP_Open(&(server->ip));
  if (!server->sock_server)
    exit(printf("SDL_net ERROR while starting menu server: %s\n",
                SDLNet_GetError()));

  return server;
}

void server_accept_localhost_only(server_t *server) {
  if (server->sock)
    return;

  server->sock = SDLNet_TCP_Accept(server->sock_server);
  if (!server->sock)
    return;

  IPaddress *remote_ip = SDLNet_TCP_GetPeerAddress(server->sock);
  if (remote_ip->host != 0x100007f) {
    printf("remote IP isn't localhost, dropping connection...\n");
    SDLNet_TCP_Close(server->sock);
    server->sock = NULL;
    return;
  }

  printf("connected to meta\n");
}

void server_frame(server_t *server) {
  server_accept_localhost_only(server);
  if (!server->sock)
    return;
}

void server_cleanup(server_t *server) {
  if (server->sock) {
    SDLNet_TCP_Send(server->sock, G2HR_MENU_META_CLEANUP,
                    sizeof(G2HR_MENU_META_CLEANUP));
    SDLNet_TCP_Close(server->sock);
  }
  SDLNet_TCP_Close(server->sock_server);
  free(server);
}
