#pragma once
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

net_t *net_init(int native_port);
void net_cleanup(net_t *net);
void net_frame(net_t *net);
void net_send_to_meta(net_t *net, char *message, char do_free);
