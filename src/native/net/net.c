#include "net.h"
#include <SDL2/SDL_net.h>

// listen on all addresses. Better would be localhost only, but SDL2_net
// doesn't support this. We'll check for localhost when establishing
// the connection though.
net_t *net_init() {
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

void net_accept_localhost_only(net_t *net) {
  if (net->sock_menu)
    return;

  net->sock_menu = SDLNet_TCP_Accept(net->sock_server);
  if (!net->sock_menu)
    return;

  IPaddress *remote_ip = SDLNet_TCP_GetPeerAddress(net->sock_server);
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

// check net->sock_menu for success
void net_block_until_connected(net_t *net, uint32_t timeout_in_ms) {
  uint32_t start_time = SDL_GetTicks();
  while (!net->sock_menu) {
    if (SDL_GetTicks() - start_time > timeout_in_ms)
      break;

    net_accept_localhost_only(net);

    SDL_Delay(100);
  }
}

// TODO: free sockets?
void net_cleanup(net_t *net) {
  if (net->sock_server)
    SDLNet_TCP_Close(net->sock_server);
  if (net->sock_menu)
    SDLNet_TCP_Close(net->sock_menu);

  for (int i = 0; i < GTA2_PLAYER_COUNT; i++)
    if (net->sock_injected[i])
      SDLNet_TCP_Close(net->sock_injected[i]);

  SDLNet_FreeSocketSet(net->set);
  free(net);
}
