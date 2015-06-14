#include "native_net.h"

char net_frame(net_t *net, void *inmenu_recv_callback, void *inmenu_userdata,
               void *ingame_recv_callback, void *ingame_userdata) {
  // hide the "esc message", when the flag is set
  for (int i = 0; i < net->injected_count; i++) {
    net_injected_instance_t *instance = net->injected_instances[i];
    if (instance->msg_clear_flag)
      net_injected_msg_clear(instance);
  }

  // check for new sockets from injected GTA2 instances
  net_accept_localhost_only(net);

  // check if the connected socket have new data
  if (!SDLNet_CheckSockets(net->set, 0))
    return 1;

  char msg_id;
  if (SDLNet_SocketReady(net->sock_menu)) {
    if (SDLNet_TCP_Recv(net->sock_menu, &msg_id, 1) <= 0) {
      printf("[native] unexpected disconnect from menu,"
             " shutting down...\n");
      return 0;
    }

    // execute the callback
    void (*func)(char msg_id, TCPsocket sock, void *userdata) =
        inmenu_recv_callback;
    func(msg_id, net->sock_menu, inmenu_userdata);
  }
  for (int i = 0; i < net->injected_count; i++) {
    net_injected_instance_t *instance = net->injected_instances[i];
    TCPsocket sock = instance->sock;

    if (!SDLNet_SocketReady(sock))
      continue;

    if (SDLNet_TCP_Recv(sock, &msg_id, 1) <= 0) {
      net_injected_instance_cleanup(net, i);
      continue;
    }

    // execute the callback
    void (*func)(char msg_id, net_injected_instance_t *instance,
                 void *userdata) = ingame_recv_callback;
    func(msg_id, instance, ingame_userdata);
  }

  return 1;
}
