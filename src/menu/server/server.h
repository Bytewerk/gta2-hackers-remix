#pragma once
#include <SDL2/SDL_net.h>

#define G2HR_MENU_SERVER_PORT_START 19990
#define G2HR_MENU_SERVER_BUFFER_LEN 100

typedef struct {
  IPaddress ip;
  TCPsocket sock_server;
  TCPsocket sock;

  // to make the tcp stuff non-blocking, we need a socket set in
  // SDL_net (otherwise SDLNet_SocketReady won't work!), even though
  // we only care about one socket.
  SDLNet_SocketSet set;

  int port;
} server_t;

server_t *server_init();
void server_cleanup(server_t *server);
void server_frame(server_t *server);
void server_send(server_t *server, char *message, char do_free);
