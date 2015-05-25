#pragma once
#include "../../common/net_message_macros_SDL.h"
#include <SDL2/SDL_net.h>

#define G2HR_MENU_SERVER_PORT_START 20150
#define G2HR_MENU_SERVER_BUFFER_LEN 100

typedef struct {
  void *userdata;

  // callback(char* message, void* userdata)
  void *meta_recv_callback;

  // callback(TCPsocket sock_native, char header, void* ud)
  void *native_recv_callback;

  IPaddress ip;
  TCPsocket sock_listen;
  TCPsocket sock_meta;
  TCPsocket sock_native;

  // to make the tcp stuff non-blocking, we need a socket set in
  // SDL_net (otherwise SDLNet_SocketReady won't work!)
  SDLNet_SocketSet set;

  int server_port;
} net_t;

net_t *net_init();
void net_cleanup(net_t *net);
char net_frame(net_t *net); // 1: success, 0: failure (disconnect)
void net_send_to_meta(net_t *net, char *message, char do_free);
