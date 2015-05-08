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

void net_frame(net_t *net, native_t *native) {
  if (!SDLNet_CheckSockets(net->set, 0))
    return;

  char header;
  if (SDLNet_SocketReady(net->sock_menu)) {
    if (SDLNet_TCP_Recv(net->sock_menu, &header, 1) <= 0) {
      printf("[meta] unexpected disconnect from menu, shutting down...\n");
      native->quit = 1;
      return;
    }
    net_frame_action_for_menu(native, header);
  }

  // TODO: also loop over the injected sockets
}
