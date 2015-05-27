#include "menu_net.h"
#include "../../common/headers/api_native2menu.h"
#include "../../common/headers/common.h"
#include <SDL2/SDL_net.h>

net_t *net_init() {
  net_t *net = calloc(1, sizeof(net_t));
  net->set = SDLNet_AllocSocketSet(2);

  // join the 'native' component's server
  SDLNet_ResolveHost(&(net->ip), "localhost", G2HR_NATIVE_SERVER_PORT);
  net->sock_native = SDLNet_TCP_Open(&(net->ip));
  if (!net->sock_native && !strcmp(SDL_GetPlatform(), "Windows"))
    exit(SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR, "G2HR",
        "Menu: Can't connect to the 'native' component!\nGTA2: Hacker's Remix "
        "is divided into multiple\ncomponents, which need to connect to each "
        "other over TCP\nvia localhost in order to do their unholy vodoo "
        "magic.\nCheck your firewall settings!\n\nMore info: "
        "http://git.io/g2hr-firewall",
        NULL));

  if (net->sock_native)
    SDLNet_TCP_AddSocket(net->set, net->sock_native);

  // listen on all addresses. Better would be localhost only,
  // but SDL2_net doesn't support this. We'll check for localhost
  // when establishing the connection though.
  for (net->server_port = G2HR_MENU_SERVER_PORT_START;
       net->server_port < G2HR_MENU_SERVER_PORT_START + 10;
       net->server_port++) {
    SDLNet_ResolveHost(&(net->ip), NULL, net->server_port);
    net->sock_listen = SDLNet_TCP_Open(&(net->ip));
    if (net->sock_listen)
      break;
  }

  if (!net->sock_listen)
    exit(printf("SDL_net ERROR while starting menu server: %s\n",
                SDLNet_GetError()));

  printf("started menu server on port %i\n", net->server_port);
  return net;
}

void net_accept_localhost_only(net_t *net) {
  net->sock_meta = SDLNet_TCP_Accept(net->sock_listen);
  if (!net->sock_meta)
    return;

  IPaddress *remote_ip = SDLNet_TCP_GetPeerAddress(net->sock_meta);
  if (remote_ip->host != 0x100007f) {
    printf("remote IP isn't localhost, dropping connection...\n");
    SDLNet_TCP_Close(net->sock_meta);
    net->sock_meta = NULL;
    return;
  }

  // add it to the socket set
  SDLNet_TCP_AddSocket(net->set, net->sock_meta);

  // drop the server socket, we only want one connection anyway!
  SDLNet_TCP_Close(net->sock_listen);
}

// returns 1 on success, 0 on failure
char net_parse_meta(net_t *net) {
  char buffer[G2HR_MENU_SERVER_BUFFER_LEN + 1];
  buffer[G2HR_MENU_SERVER_BUFFER_LEN] = '\0';

  int length =
      SDLNet_TCP_Recv(net->sock_meta, &buffer, G2HR_MENU_SERVER_BUFFER_LEN);

  if (length < 0) {
    printf("ERROR while receiving data from meta component!\n");
    return 0;
  }
  printf("[meta => menu]: %s\n", buffer);

  void (*callback)(char *msg, void *ud) = net->meta_recv_callback;
  callback(buffer, net->userdata);
  return 1;
}

// returns 1 on success, 0 on failure
char net_parse_native(net_t *net) {
  char header;

  if (SDLNet_TCP_Recv(net->sock_native, &header, 1) <= 0) {
    printf("[menu] unexpected disconnect from native!\n");
    return 0;
  }

  void (*callback)(TCPsocket sock_native, char header, void *ud) =
      net->native_recv_callback;
  callback(net->sock_native, header, net->userdata);
  return 1;
}

// returns 1 on success, 0 on failure
char net_frame(net_t *net) {
  if (!net->sock_meta)
    net_accept_localhost_only(net);

  if (!SDLNet_CheckSockets(net->set, 0))
    return 1;

  if (net->sock_meta && SDLNet_SocketReady(net->sock_meta))
    if (!net_parse_meta(net))
      return 0;

  if (net->sock_native && SDLNet_SocketReady(net->sock_native))
    if (!net_parse_native(net))
      return 0;

  return 1;
}

void net_send_to_meta(net_t *net, char *message, char do_free) {
  printf("[menu => meta%s]: %s\n", net->sock_meta ? "" : " (disconnected)",
         message);

  if (net->sock_meta)
    SDLNet_TCP_Send(net->sock_meta, message, strlen(message));

  if (do_free)
    free(message);
}

void net_send_to_native(net_t *net, char message) {
  printf("[menu => native%s]: code 0x%x (see api_native2menu.h)\n",
         net->sock_native ? "" : " (disconnected)", message);

  if (net->sock_native)
    SDLNet_TCP_Send(net->sock_native, &message, 1);
}

void net_cleanup(net_t *net) {
  // send cleanup to meta and native
  net_send_to_meta(net, "CLEANUP", 0);
  net_send_to_native(net, NA_CLEANUP);

  if (net->sock_listen)
    SDLNet_TCP_Close(net->sock_listen);
  if (net->sock_meta)
    SDLNet_TCP_Close(net->sock_meta);
  if (net->sock_native)
    SDLNet_TCP_Close(net->sock_native);

  SDLNet_FreeSocketSet(net->set);
  free(net);
}
