#pragma once
#include <SDL2/SDL_net.h>

#define G2HR_NATIVE_SERVER_PORT_START 20140
#define GTA2_PLAYER_COUNT 6 // TODO: put in common.h

typedef struct {
  IPaddress ip;
  TCPsocket sock_server;
  TCPsocket sock_menu;
  TCPsocket sock_injected[GTA2_PLAYER_COUNT];

  SDLNet_SocketSet set;

  int port;
} net_t;

net_t *net_init();

// net_connect_injected(int count);

void net_cleanup(net_t *net);
