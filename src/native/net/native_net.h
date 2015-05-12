#pragma once
#include "../../common/common.h"
#include "../native.h"
#include <SDL2/SDL_net.h>

typedef struct { TCPsocket tcp; } net_sock_t;

typedef struct {
  IPaddress ip;
  TCPsocket sock_listen;
  TCPsocket sock_menu;

  // all 'injected' clients, sorted by connection time
  TCPsocket sock_injected_sorted_by_time[GTA2_PLAYER_COUNT];

  // 'injected' clients, sorted by player ID
  TCPsocket sock_injected_sorted_by_id[GTA2_PLAYER_COUNT];

  // TODO: we'll probably need to recreate this set, once
  // an instance disconnects!
  SDLNet_SocketSet set;

  int sock_injected_count;
} net_t;

net_t *net_init();

void net_block_until_connected(net_t *net, uint32_t timeout_in_ms);

void net_cleanup(net_t *net);

void net_frame(net_t *net, native_t *native);
