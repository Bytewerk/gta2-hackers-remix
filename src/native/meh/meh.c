#include "meh.h"
#include "../../common/native_api.h"

#define MEH_SEND_HEADER(WHICH)                                                 \
  {                                                                            \
    char header = WHICH;                                                       \
    SDLNet_TCP_Send(net->sock_menu, &header, 1);                               \
  }

void meh_send_action(net_t *net, tk_action_t action, char redraw) {
  MEH_SEND_HEADER(NA_ACTION);
  NA_ACTION_t data = {action, redraw};
  SDLNet_TCP_Send(net->sock_menu, &data, sizeof(data));
}

void meh_frame(net_t *net, pads_t *pads, SDL_Event *event) {
  SDL_EventType t = event->type;

  // send the controller count
  if (t == SDL_CONTROLLERDEVICEADDED || t == SDL_CONTROLLERDEVICEREMOVED) {
    MEH_SEND_HEADER(NA_CONTROLLERS_CONNECTED);
    NA_CONTROLLERS_CONNECTED_t data = {pads->count};
    SDLNet_TCP_Send(net->sock_menu, &data, sizeof(data));
  }
}

#undef MEH_SEND_HEADER
