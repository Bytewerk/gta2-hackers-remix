#include "inmenu.h"
#include "../../common/headers/api_native2menu.h"

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
      printf("; SP: %i\n", data->singleplayer);

      inmenu->singleplayer = data->singleplayer;
    });
  }
}

#define MAP_BUTTON(BUTTON, TK_ACTION)                                          \
  if (b == SDL_CONTROLLER_BUTTON_##BUTTON)                                     \
  MESSAGESEND(inmenu->net->sock_menu, NA_ACTION, {                             \
    data->action = TK_ACTION_##TK_ACTION;                                      \
    data->redraw = 1;                                                          \
  })

void inmenu_analogstick_action(inmenu_t *inmenu, tk_action_t action) {
  uint32_t current_time = SDL_GetTicks();
  if (current_time - inmenu->analogstick_timer[action] <
      G2HR_ANALOG_STICK_MENU_LIMIT_RATE)
    return;
  inmenu->analogstick_timer[action] = current_time;

  MESSAGESEND(inmenu->net->sock_menu, NA_ACTION, {
    data->action = action;
    data->redraw = 1;
  });
}

void inmenu_frame_timeout(inmenu_t *in) {
  if (in->pads->count < 1)
    return;

  SDL_GameController *controller = in->pads->first->controller;

  int16_t deadzone = 32767 / 2;
  int16_t x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
  int16_t y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

  if (x < -1 * deadzone)
    inmenu_analogstick_action(in, TK_ACTION_LEFT);
  if (x > deadzone)
    inmenu_analogstick_action(in, TK_ACTION_RIGHT);
  if (y < -1 * deadzone)
    inmenu_analogstick_action(in, TK_ACTION_UP);
  if (y > deadzone)
    inmenu_analogstick_action(in, TK_ACTION_DOWN);

  return;
}

void inmenu_frame(inmenu_t *inmenu, SDL_Event *event) {
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
#undef SEND_ACTION

void inmenu_cleanup(inmenu_t *inmenu) { free(inmenu); }
