#include "../../common/native_api.h"
#include "native_net.h"

void net_frame_action_for_menu(native_t *native, char action) {
  switch (action) {
  case NA_CLEANUP:
    native->quit = 1;
    break;
  case NA_POWEROFF:
    printf("[menu => native] POWEROFF (not implemented yet!)\n");
    native->quit = 1;
    break;
  case NA_REBOOT:
    printf("[menu => native] REBOOT (not implemented yet!)\n");
    native->quit = 1;
    break;
  }
}

void net_frame_action_for_injected_instance(native_t *native, int id,
                                            char action) {
  printf("[injected#%i => native] %c\n", id, action);
}

void net_frame(net_t *net, native_t *native) {
  // check for new sockets from injected GTA2 instances
  net_accept_localhost_only(net);

  // check if the connected socket have new data
  if (!SDLNet_CheckSockets(net->set, 0))
    return;

  char header;
  if (SDLNet_SocketReady(net->sock_menu)) {
    if (SDLNet_TCP_Recv(net->sock_menu, &header, 1) <= 0) {
      printf("[native] unexpected disconnect from menu, shutting down...\n");
      native->quit = 1;
      return;
    }
    net_frame_action_for_menu(native, header);
  }
  for (int i = 0; i < net->gta2_session_count; i++) {
    TCPsocket sock = net->gta2_sessions[i]->sock;
    if (!SDLNet_SocketReady(sock))
      continue;

    if (SDLNet_TCP_Recv(sock, &header, 1) <= 0) {
      net_gta2_session_cleanup(net, i);
      continue;
    }

    net_frame_action_for_injected_instance(native, i, header);
  }
}
