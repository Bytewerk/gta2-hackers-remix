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

  switch (msg_id) {
    MESSAGECASESHORT(NA_CLEANUP, inmenu->has_quit = 1);
    MESSAGECASESHORT(NA_POWEROFF, {
      printf("[menu => native] POWEROFF (not implemented yet!)"
             "\n");
      inmenu->has_quit = 1;
    });
    MESSAGECASESHORT(NA_REBOOT, {
      printf("[menu => native] REBOOT (not implemented yet!)\n");
      inmenu->has_quit = 1;
    });

    MESSAGECASE(sock, NA_PID_TABLE, {
      for (int i = 0; i < 6; i++)
        inmenu->pids[i] = data->pids[i];
      printf("[native] got pid table:");
      for (int i = 0; i < 6; i++)
        printf(" %i", data->pids[i]);
      printf("\n");
    });
  }
}

#define MAP_BUTTON(BUTTON, TK_ACTION)                                          \
  if (b == SDL_CONTROLLER_BUTTON_##BUTTON)                                     \
  MESSAGESEND(inmenu->net->sock_menu, NA_ACTION, {                             \
    data->action = TK_ACTION_##TK_ACTION;                                      \
    data->redraw = 1;                                                          \
  })

void inmenu_frame(inmenu_t *inmenu, SDL_Event *event) {
  if (!event)
    return;

  SDL_EventType t = event->type;

  // send the controller count
  if (t == SDL_CONTROLLERDEVICEADDED || t == SDL_CONTROLLERDEVICEREMOVED) {
    MESSAGESEND(inmenu->net->sock_menu, NA_CONTROLLERS_CONNECTED,
                data->count = inmenu->pads->count);
  }

  if (t == SDL_CONTROLLERBUTTONDOWN &&
      event->cbutton.type == SDL_CONTROLLERBUTTONDOWN) {
    SDL_GameControllerButton b = event->cbutton.button;

    MAP_BUTTON(DPAD_UP, UP);
    MAP_BUTTON(DPAD_LEFT, LEFT);
    MAP_BUTTON(DPAD_RIGHT, RIGHT);
    MAP_BUTTON(DPAD_DOWN, DOWN);
    MAP_BUTTON(A, ENTER);
    MAP_BUTTON(START, ENTER);
    MAP_BUTTON(B, ESC);
    MAP_BUTTON(BACK, ESC);
  }
}
#undef MAP_BUTTON

void inmenu_cleanup(inmenu_t *inmenu) { free(inmenu); }
