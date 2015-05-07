#include "server.h"
#include <SDL2/SDL_net.h>

// listen on all addresses. Better would be localhost only, but SDL2_net
// doesn't support this. We'll check for localhost when establishing
// the connection though.
server_t *server_init() {
  server_t *server = malloc(sizeof(server_t));
  server->set = SDLNet_AllocSocketSet(1);
  server->sock = 0;

  for (server->port = G2HR_MENU_SERVER_PORT_START;
       server->port < G2HR_MENU_SERVER_PORT_START + 10; server->port++) {
    SDLNet_ResolveHost(&(server->ip), NULL, server->port);
    server->sock_server = SDLNet_TCP_Open(&(server->ip));
    if (server->sock_server)
      break;
  }

  if (!server->sock_server)
    exit(printf("SDL_net ERROR while starting menu server: %s\n",
                SDLNet_GetError()));

  printf("started menu server on port %i\n", server->port);
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

  // add it to the socket set
  SDLNet_TCP_AddSocket(server->set, server->sock);

  // drop the server socket, we only want one connection anyway!
  SDLNet_TCP_Close(server->sock_server);
}

char server_parse(server_t *server) {
  char buffer[G2HR_MENU_SERVER_BUFFER_LEN + 1];
  buffer[G2HR_MENU_SERVER_BUFFER_LEN] = '\0';

  int length =
      SDLNet_TCP_Recv(server->sock, &buffer, G2HR_MENU_SERVER_BUFFER_LEN);

  if (length < 0) {
    printf("ERROR while receiving data from meta component!");
    return 0;
  }
  printf("[meta => menu]: %s\n", buffer);
  return 1;
}

void server_frame(server_t *server) {
  server_accept_localhost_only(server);
  if (!server->sock || !SDLNet_CheckSockets(server->set, 0) ||
      !SDLNet_SocketReady(server->sock))
    return;
  server_parse(server);
}

void server_send(server_t *server, char *message, char do_free) {
  printf("[menu => meta%s]: %s\n", server->sock ? "" : " (disconnected)",
         message);

  if (server->sock)
    SDLNet_TCP_Send(server->sock, message, strlen(message));

  if (do_free)
    free(message);
}

void server_cleanup(server_t *server) {
  server_send(server, "CLEANUP", 0);
  if (server->sock_server)
    SDLNet_TCP_Close(server->sock_server);
  if (server->sock)
    SDLNet_TCP_Close(server->sock);

  SDLNet_FreeSocketSet(server->set);
  free(server);
}
