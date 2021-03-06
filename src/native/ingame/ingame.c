#include "ingame.h"
#include "../../common/headers/api_native2injected.h"
#include "../../common/headers/api_native2menu.h"

ingame_t *ingame_init(net_t *net, cmap_t *cmap, pads_t *pads,
                      inmenu_t *inmenu) {
  ingame_t *ingame = calloc(1, sizeof(ingame_t));
  ingame->net = net;
  ingame->cmap = cmap;
  ingame->pads = pads;
  ingame->inmenu = inmenu;

  return ingame;
}

int ingame_player_id_by_pid(uint16_t pid, uint16_t *table) {
  for (int i = 0; i < 6; i++)
    if (pid == table[i])
      return i;
  return -1;
}

void ingame_recv_callback(unsigned char msg_id,
                          net_injected_instance_t *instance, void *userdata) {
  TCPsocket sock = instance->sock;
  ingame_instance_userdata_t *ud_instance =
      (ingame_instance_userdata_t *)instance->userdata;
  ingame_t *ingame = (ingame_t *)userdata;

  if (!ud_instance) {
    ud_instance = calloc(1, sizeof(ingame_instance_userdata_t));
    ud_instance->player_id = -1;
    ud_instance->cmap_selected = ingame->cmap;
    instance->userdata = (void *)ud_instance;
  }

  switch (msg_id) {
    MESSAGECASE(sock, IA_PID, {
      ud_instance->instance_pid = data->pid;

      int player_id = ingame_player_id_by_pid(data->pid, ingame->inmenu->pids);

      if (player_id > -1 && player_id < GTA2_PLAYER_COUNT) {
        ud_instance->player_id = player_id;
        printf("[native:p%i] assigned player id by pid: %i\n", player_id + 1,
               data->pid);

        MESSAGESEND(sock, IA_PLAYER_ID,
                    // todo: set the singleplayer mode boolean!
                    // data->singleplayer = ingame->singleplayer
                    data->id = ud_instance->player_id);

        ingame->instance_by_player_id[player_id] = instance;

        // store the array location, so it can get NULL'd on
        // disconnect by net_frame().
        instance->sorted_array_location =
            &(ingame->instance_by_player_id[player_id]);

      } else
        printf("[native] ERROR: got pid %i, but can't find"
               " the associated player ID!\n",
               data->pid);

    });
    MESSAGECASE(sock, IA_VEHICLE_INFO,
                printf("[native:p%i] %s a vehicle\n",
                       ud_instance->player_id + 1,
                       data->in_vehicle ? "entered" : "left");
                ud_instance->driving = data->in_vehicle;);
    MESSAGECASESHORT(
        IA_INIT_COMPLETE,
        printf("[native:p%i] initialized!\n", ud_instance->player_id + 1);

        // show the gamepad layout after the game has started
        // TODO: add an option in the menu to deactivate this
        net_injected_msg_set(instance, 5000, "Layout:",
                             ud_instance->cmap_selected->description,
                             "START: Quit       BACK: Change"););
  }
}

void ingame_send_movement_data(ingame_t *ingame,
                               net_injected_instance_t *instance,
                               pad_controller_t *pad) {
  ingame_instance_userdata_t *ud =
      (ingame_instance_userdata_t *)instance->userdata;
  cmap_state_t *state = ud->driving ? &(ud->cmap_selected->driving)
                                    : &(ud->cmap_selected->walking);

  uint16_t movement = 0;

  // handle all buttons
  for (SDL_GameControllerButton i = SDL_CONTROLLER_BUTTON_A;
       i < SDL_CONTROLLER_BUTTON_MAX; i++) {
    // not pressed: skip
    if (!SDL_GameControllerGetButton(pad->controller, i))
      continue;

    movement |= cmap_action_to_movement_bitmask(state->buttons[i]);
  }

  // handle axes
  for (SDL_GameControllerAxis i = SDL_CONTROLLER_AXIS_LEFTX;
       i < SDL_CONTROLLER_AXIS_MAX; i++) {
    int16_t value = SDL_GameControllerGetAxis(pad->controller, i);
    int16_t deadzone_positive = 32767 / 2;
    int16_t deadzone_negative = 32767 / 2;

    if (i == SDL_CONTROLLER_AXIS_LEFTX) {
      deadzone_positive = state->dead_leftstick.right;
      deadzone_negative = state->dead_leftstick.left;
    } else if (i == SDL_CONTROLLER_AXIS_LEFTY) {
      deadzone_positive = state->dead_leftstick.down;
      deadzone_negative = state->dead_leftstick.up;
    } else if (i == SDL_CONTROLLER_AXIS_RIGHTX) {
      deadzone_positive = state->dead_rightstick.right;
      deadzone_negative = state->dead_rightstick.left;
    } else if (i == SDL_CONTROLLER_AXIS_RIGHTY) {
      deadzone_positive = state->dead_rightstick.down;
      deadzone_negative = state->dead_rightstick.up;
    }

    if (value > deadzone_positive)
      movement |= cmap_action_to_movement_bitmask(state->axis_positive[i]);
    else if (value < -1 * deadzone_negative)
      movement |= cmap_action_to_movement_bitmask(state->axis_negative[i]);
  }

  // stuff it up the socket
  MESSAGESEND(instance->sock, IA_MOVEMENT, data->movement = movement);
}

void ingame_handle_buttonpress(ingame_t *ingame,
                               net_injected_instance_t *instance,
                               pad_controller_t *pad,
                               SDL_GameControllerButton button, int player_id) {
  ingame_instance_userdata_t *ud =
      (ingame_instance_userdata_t *)instance->userdata;

  if (ud->is_in_quit_dialog) {
    if (button == SDL_CONTROLLER_BUTTON_START) {
      MESSAGESEND(ingame->net->sock_menu, NA_QUIT_GAME,
                  data->player_id = player_id);
    }
    if (button == SDL_CONTROLLER_BUTTON_BACK) {
      net_injected_msg_clear(instance);
      ud->is_in_quit_dialog = 0;
    }
  } else {
    if (button == SDL_CONTROLLER_BUTTON_BACK) {
      ud->cmap_selected =
          ud->cmap_selected->next ? ud->cmap_selected->next : ingame->cmap;
      printf("[native:p%i] next layout selected: %s (%p)\n", player_id + 1,
             ud->cmap_selected->description, ud->cmap_selected);
      net_injected_msg_set(instance, 1000, "Layout:",
                           ud->cmap_selected->description, NULL);
    }
    if (button == SDL_CONTROLLER_BUTTON_START) {
      net_injected_msg_set(instance, false, "Is that it?",
                           "Press START to quit play.",
                           "Press BACK to continue.");

      ud->is_in_quit_dialog = 1;
    }
  }
}

// event: NULL when the timeout has been reached
void ingame_frame(ingame_t *ingame, SDL_Event *event) {
  int count = MIN(ingame->net->injected_count, ingame->pads->count);
  pad_controller_t *pad = ingame->pads->first;

  for (int i = 0; i < count && pad; i++) {
    net_injected_instance_t *instance = ingame->instance_by_player_id[i];

    if (instance) {
      ingame_instance_userdata_t *ud =
          (ingame_instance_userdata_t *)instance->userdata;

      if (event) {
        if (event->type == SDL_CONTROLLERBUTTONDOWN &&
            event->cbutton.which == pad->joystick_id) {
          ingame_handle_buttonpress(ingame, instance, pad,
                                    event->cbutton.button, i);
        }
      } else if (!ud->is_in_quit_dialog)
        ingame_send_movement_data(ingame, instance, pad);
    }
    pad = pad->next;
  }
}

void ingame_cleanup(ingame_t *ingame) { free(ingame); }
