#pragma once
#include "../../common/common.h"
#include "../native.h"
#include <SDL2/SDL_net.h>

typedef struct {
  TCPsocket sock;
  void *userdata;

} net_injected_instance_t;

typedef struct {
  IPaddress ip;
  TCPsocket sock_listen;
  TCPsocket sock_menu;

  // these are sorted by connection time, not by the player
  // id. that's what we have the player_id field inside the
  // struct for.
  net_injected_instance_t *injected_instances[GTA2_PLAYER_COUNT];
  int injected_count;

  int player_port_table[GTA2_PLAYER_COUNT];

  // Looks like we don't need to re-create this set, once a socket
  // disconnects. Tested with ~10 sessions, although it can hold only
  // 6 of them.
  SDLNet_SocketSet set;

} net_t;

net_t *net_init();

void net_block_until_connected(net_t *net, uint32_t timeout_in_ms);

void net_accept_localhost_only(net_t *net);

/*
        returns 0 on error.

        inmenu_recv_callback parameters:
                unsigned char msg_id, TCPsocket sock, void* userdata

        ingame_recv_callback parameters:
                unsigned char msg_id, net_injected_instance_t* instance

        We need different callback parameters, because the ingame function
        must be able to allocate new usedata dynamically and attach it to
        the instance.
*/
char net_frame(net_t *net, void *inmenu_recv_callback, void *inmenu_userdata,
               void *ingame_recv_callback);

void net_injected_instance_cleanup(net_t *net, int id);

void net_cleanup(net_t *net);

#define NET_SEND_HEADER(SOCKET, HEADER)                                        \
  {                                                                            \
    char header = HEADER;                                                      \
    SDLNet_TCP_Send(SOCKET, &header, 1);                                       \
  }
