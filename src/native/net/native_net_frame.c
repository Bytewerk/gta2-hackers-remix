#include "../../common/api_native2injected.h"
#include "../../common/api_native2menu.h"
#include "native_net.h"

void net_frame_action_for_menu(native_t *native, char msg_id) {
  switch (msg_id) {
  case NA_CLEANUP:
    native->quit = 1;
    break;
  case NA_POWEROFF:
    printf("[menu => native] POWEROFF"
           " (not implemented yet!)\n");
    native->quit = 1;
    break;
  case NA_REBOOT:
    printf("[menu => native] REBOOT (not implemented yet!)\n");
    native->quit = 1;
    break;
  default:
    printf("[menu => native] %i\n", msg_id);
  }
}

// do something with 'data' in the CODE section
// 'break' out of the case if the data is garbage!
#define FRAMEDATACASE(MSG_ID, CODE)                                            \
  case MSG_ID: {                                                               \
    char buffer[sizeof(MSG_ID##_t)];                                           \
    SDLNet_TCP_Recv(session->sock, &buffer, sizeof(MSG_ID##_t));               \
    MSG_ID##_t *data = (MSG_ID##_t *)&buffer;                                  \
    CODE return;                                                               \
  }

void net_frame_action_for_injected_instance(net_gta2_session_t *session,
                                            char msg_id) {
  switch (msg_id) {
    FRAMEDATACASE(IA_PID, {
      session->instance_pid = data->pid;
      printf("got pid: %i\n", data->pid);

      // TODO: try to look up player number by pid now
      // STUB STUB STUB BAD CODE :p
      NET_SEND_HEADER(session->sock, IA_PLAYER_ID);
      char player_id = 1;
      NET_SEND_HEADER(session->sock, player_id);
    });
  }
  // TODO: if we're here, it was garbage, show a warning.
}

#undef FRAMEDATACASE

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
    net_gta2_session_t *session = net->gta2_sessions[i];
    TCPsocket sock = session->sock;
    if (!SDLNet_SocketReady(sock))
      continue;

    if (SDLNet_TCP_Recv(sock, &header, 1) <= 0) {
      net_gta2_session_cleanup(net, i);
      continue;
    }

    net_frame_action_for_injected_instance(session, header);
  }
}
