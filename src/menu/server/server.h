#pragma once
#include <SDL2/SDL_net.h>

#define G2HR_MENU_SERVER_PORT_START 19990

typedef struct {
  IPaddress ip;
  TCPsocket sock_server;
  TCPsocket sock;
  int port;
} server_t;

server_t *server_init();
void server_cleanup(server_t *server);
void server_frame(server_t *server);
void server_send(server_t *server, char *message, char do_free);
