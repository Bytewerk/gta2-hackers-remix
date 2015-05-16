#include "inmenu.h"
#include "../../common/api_native2menu.h"

inmenu_t *inmenu_init(net_t *net, pads_t *pads) {
  inmenu_t *inmenu = calloc(1, sizeof(inmenu_t));
  inmenu->net = net;
  inmenu->pads = pads;
  return inmenu;
}

void inmenu_recv_callback(unsigned char msg_id, TCPsocket sock,
                          void *userdata) {
  inmenu_t *inmenu = (inmenu_t *)userdata;

  // FIXME: replace with FRAMEDATACASE-like macro syntax
  switch (msg_id) {
  case NA_CLEANUP:
    inmenu->has_quit = 1;
    break;
  case NA_POWEROFF:
    printf("[menu => native] POWEROFF"
           " (not implemented yet!)\n");
    inmenu->has_quit = 1;
    break;
  case NA_REBOOT:
    printf("[menu => native] REBOOT (not implemented yet!)\n");
    inmenu->has_quit = 1;
    break;
  default:
    printf("[menu => native] %i\n", msg_id);
  }
}

// FIXME: replace with SEND2MENU macro syntax!
void inmenu_send_action(net_t *net, tk_action_t action, char redraw) {
  NET_SEND_HEADER(net->sock_menu, NA_ACTION);
  NA_ACTION_t data = {action, redraw};
  SDLNet_TCP_Send(net->sock_menu, &data, sizeof(data));
}

#define MAP_BUTTON(BUTTON, TK_ACTION)                                          \
  if (b == SDL_CONTROLLER_BUTTON_##BUTTON)                                     \
    inmenu_send_action(inmenu->net, TK_ACTION_##TK_ACTION, 1);

void inmenu_frame(inmenu_t *inmenu, SDL_Event *event) {
  SDL_EventType t = event->type;

  // send the controller count
  if (t == SDL_CONTROLLERDEVICEADDED || t == SDL_CONTROLLERDEVICEREMOVED) {
    NET_SEND_HEADER(inmenu->net->sock_menu, NA_CONTROLLERS_CONNECTED);
    NA_CONTROLLERS_CONNECTED_t data = {inmenu->pads->count};
    SDLNet_TCP_Send(inmenu->net->sock_menu, &data, sizeof(data));
  }

  if (t == SDL_CONTROLLERBUTTONDOWN &&
      event->cbutton.type == SDL_CONTROLLERBUTTONDOWN) {
    SDL_GameControllerButton b = event->cbutton.button;

    MAP_BUTTON(DPAD_UP, UP);
    MAP_BUTTON(DPAD_LEFT, LEFT);
    MAP_BUTTON(DPAD_RIGHT, RIGHT);
    MAP_BUTTON(DPAD_DOWN, DOWN);
    MAP_BUTTON(A, ENTER);
    MAP_BUTTON(B, ESC);
  }
}
#undef MAP_BUTTON

void inmenu_cleanup(inmenu_t *inmenu) { free(inmenu); }
