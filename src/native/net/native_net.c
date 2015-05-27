#include "native_net.h"
#include "../../common/headers/api_native2menu.h"
#include <SDL2/SDL_net.h>

// listen on all addresses. Better would be localhost only, but SDL2_net
// doesn't support this. We'll check for localhost when establishing
// the connection though.
net_t *net_init() {
  net_t *net = calloc(1, sizeof(net_t));
  net->set = SDLNet_AllocSocketSet(GTA2_PLAYER_COUNT + 1);

  SDLNet_ResolveHost(&(net->ip), NULL, G2HR_NATIVE_SERVER_PORT);
  net->sock_listen = SDLNet_TCP_Open(&(net->ip));

  if (!net->sock_listen)
    exit(printf("SDL_net ERROR while starting native server on"
                "port %i: %s\n",
                G2HR_NATIVE_SERVER_PORT, SDLNet_GetError()));
  return net;
}

void net_accept_localhost_only(net_t *net) {
  TCPsocket new = SDLNet_TCP_Accept(net->sock_listen);
  if (!new)
    return;

  IPaddress *remote_ip = SDLNet_TCP_GetPeerAddress(new);
  if (remote_ip->host != 0x100007f) {
    printf("[native] incoming connection, but not from localhost."
           " Drop it like it's hot!\n");
    SDLNet_TCP_Close(new);
    return;
  }

  // the first socket is always the menu socket!
  if (!net->sock_menu) {
    printf("[native] connected to menu\n");
    net->sock_menu = new;
  } else if (net->injected_count < GTA2_PLAYER_COUNT - 1) {
    net_injected_instance_t *instance =
        calloc(1, sizeof(net_injected_instance_t));

    instance->sock = new;
    net->injected_instances[net->injected_count] = instance;
    net->injected_count++;

    printf("[native] connection established to injected GTA2"
           " instance (count: %i)\n",
           net->injected_count);
  } else {
    printf("[native] WARNING: we already have %i connected GTA2"
           " instances! Something wrong, dropping the new connection!"
           " Please report this at http://git.io/g2hr-bugs\n",
           net->injected_count);
    SDLNet_TCP_Close(new);
    return;
  }

  // add it to the socket set
  SDLNet_TCP_AddSocket(net->set, new);
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

void net_injected_instance_cleanup(net_t *net, int id) {
  net->injected_count--;
  printf("[native] injected GTA2 instance %i disconnected"
         " (count: %i)\n",
         id, net->injected_count);

  net_injected_instance_t *instance = net->injected_instances[id];
  SDLNet_TCP_Close(instance->sock);
  if (instance->sorted_array_location)
    *(instance->sorted_array_location) = NULL;
  if (instance->userdata)
    free(instance->userdata);
  free(instance);

  // move up all later instances in the array
  for (int i = id + 1; i <= net->injected_count; i++)
    net->injected_instances[i - 1] = net->injected_instances[i];
}

void net_cleanup(net_t *net) {
  if (net->sock_listen)
    SDLNet_TCP_Close(net->sock_listen);
  if (net->sock_menu)
    SDLNet_TCP_Close(net->sock_menu);

  for (int i = 0; i < net->injected_count; i++)
    net_injected_instance_cleanup(net, i);

  SDLNet_FreeSocketSet(net->set);
  free(net);
}
