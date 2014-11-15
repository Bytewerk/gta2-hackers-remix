#include "game.h"
#include "gta2_controls.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define AXIS_DEADZONE 10000

short map_button(SDL_GameController *pad, SDL_GameControllerButton button,
                 short ctrl_bit) {
  return SDL_GameControllerGetButton(pad, button) ? ctrl_bit : 0;
}

short map_axis(SDL_GameController *pad, SDL_GameControllerAxis axis,
               short ctrl_positive, short ctrl_negative) {
  int offset = SDL_GameControllerGetAxis(pad, axis);
  if (offset >= AXIS_DEADZONE)
    return ctrl_positive;
  if (offset <= AXIS_DEADZONE * -1)
    return ctrl_negative;
  return 0;
}

short map_trigger(SDL_GameController *pad, SDL_GameControllerAxis trigger,
                  short ctrl_bit) {
  return SDL_GameControllerGetAxis(pad, trigger) > AXIS_DEADZONE ? ctrl_bit : 0;
}

/*
        How to add a new mapping:
        * increase mapping_count by 1
        * add a mapping_names ... line
        * add your mapping to the switch... case below
          (don't forget the break; at the end!)
        * if it is a good mapping, do a pull request
          on github (or post it to the gtamp thread)
*/
short controller_mapping(SDL_Event *e, player_t *player) {
  int mapping_count = 2;
  const char *mapping_names[mapping_count];
  mapping_names[0] = "(Default) robotanarchy";
  mapping_names[1] = "Cuban-Pete";

  // The guide button rotates through the mappings
  if (e->type == SDL_CONTROLLERBUTTONDOWN &&
      e->cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE &&
      e->cbutton.which == player->joystick_id) {
    player->mapping++;
    player->mapping %= mapping_count;
    printf("Some player switched to %s's mapping (%i/%i)!\n",
           mapping_names[player->mapping], player->mapping + 1, mapping_count);
  }

  short ret = GTA2_CTRL_NOT_MOVING;
  switch (player->mapping) {
  /*
                  Default (robotanarchy's mapping):
                          Triggers L/R: forward / backward
                          Left stick L/R: go left, right
                          DPAD L/R: previous, next weapons
                          A: Shoot
                          B: Jump / Handbrake
          X: Enter Car
          LSB: Special 1
          RSB: Special 2
  */
  case 0:
    ret |= map_trigger(player->pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
                       GTA2_CTRL_FORWARD);
    ret |= map_trigger(player->pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT,
                       GTA2_CTRL_BACKWARD);
    ret |= map_axis(player->pad, SDL_CONTROLLER_AXIS_LEFTX, GTA2_CTRL_RIGHT,
                    GTA2_CTRL_LEFT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT,
                      GTA2_CTRL_WEAPON_PREV);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
                      GTA2_CTRL_WEAPON_NEXT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_A, GTA2_CTRL_ATTACK);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_B,
                      GTA2_CTRL_JUMP_HANDBRAKE);
    ret |=
        map_button(player->pad, SDL_CONTROLLER_BUTTON_X, GTA2_CTRL_ENTER_EXIT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                      GTA2_CTRL_SPECIAL_1);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
                      GTA2_CTRL_SPECIAL_2);
    break;

  case 1:
    /*
                        Cuban-Pete's mapping:
                        http://gtamp.com/forum/viewtopic.php?p=9199#p9201

                        What is not possible right now:
                                - Escape/Quit, Pause, Settings/Menu (all of this
       is the same anyway?)
                                - Change Radio (relevant for single player)
                                - Last Message
                                - Independent camera movement

                        FIXME: Not sure if the B button should be special 1 or
       special 2
    */
    ret |= map_trigger(player->pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT,
                       GTA2_CTRL_JUMP_HANDBRAKE);
    ret |= map_trigger(player->pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
                       GTA2_CTRL_ATTACK);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                      GTA2_CTRL_WEAPON_PREV);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
                      GTA2_CTRL_WEAPON_NEXT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_UP,
                      GTA2_CTRL_FORWARD);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN,
                      GTA2_CTRL_BACKWARD);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT,
                      GTA2_CTRL_LEFT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
                      GTA2_CTRL_RIGHT);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_A, GTA2_CTRL_ATTACK);
    ret |=
        map_button(player->pad, SDL_CONTROLLER_BUTTON_B, GTA2_CTRL_SPECIAL_2);
    ret |= map_button(player->pad, SDL_CONTROLLER_BUTTON_X,
                      GTA2_CTRL_JUMP_HANDBRAKE);
    ret |=
        map_button(player->pad, SDL_CONTROLLER_BUTTON_Y, GTA2_CTRL_ENTER_EXIT);

    break;
  }
  return ret;
}
