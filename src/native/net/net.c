#include "net.h"
#include <SDL2/SDL_net.h>

// listen on all addresses. Better would be localhost only, but SDL2_net
// doesn't support this. We'll check for localhost when establishing
// the connection though.
net_t *server_init() {
  net_t *net = calloc(1, sizeof(net_t));
  net->set = SDLNet_AllocSocketSet(GTA2_PLAYER_COUNT + 1);

  for (net->port = G2HR_NATIVE_SERVER_PORT_START;
       net->port < G2HR_NATIVE_SERVER_PORT_START + 10; net->port++) {
    SDLNet_ResolveHost(&(net->ip), NULL, net->port);
    net->sock_server = SDLNet_TCP_Open(&(net->ip));
    if (net->sock_server)
      break;
  }

  if (!net->sock_server)
    exit(printf("SDL_net ERROR while starting native server: %s\n",
                SDLNet_GetError()));

  printf("started native server on port %i\n", net->port);
  return net;
}

void net_accept_localhost_only(net_t *server) {
  if (net->sock_menu)
    return;

  net->sock_menu = SDLNet_TCP_Accept(net->sock);
  if (!net->sock_menu)
    return;

  IPaddress *remote_ip = SDLNet_TCP_GetPeerAddress(server->sock);
  if (remote_ip->host != 0x100007f) {
    printf("remote IP isn't localhost, dropping connection...\n");
    SDLNet_TCP_Close(net->sock_menu);
    net->sock_menu = NULL;
    return;
  }

  // add it to the socket set
  SDLNet_TCP_AddSocket(net->set, net->sock_menu);

  // drop the server socket, we only want one connection anyway!
  SDLNet_TCP_Close(net->sock_server);
}

void net_cleanup(net_t *net) {
  if (server->sock_server)
    SDLNet_TCP_Close(server->sock_server);
  if (server->sock_menu)
    SDLNet_TCP_Close(server->sock_menu);

  for (int i = 0; i < GTA2_PLAYER_COUNT; i++)
    if (server->sock_injected[i])
      SDLNet_TCP_Close(server->sock_injected[i]);

  SDLNet_FreeSocketSet(net->set);
  free(server);
}
