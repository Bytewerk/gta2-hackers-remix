#include "game.h"
#include "gta2_controls.h"
#include <SDL2/SDL.h>

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

short controller_mapping(game_t *game) {
  short ret = GTA2_CTRL_NOT_MOVING;

  // TODO: check for the mapping change key
  // (but only if it was pressed and released)

  switch (game->mapping) {
  /*
                  Default:
                          triggers L/R: forward / backward
                          left stick L/R: go left, right
                          DPAD L/R: previous, next weapons
                          A: Shoot
                          B: Jump / Handbrake
          X: Enter Car
          LSB: Special 1
          RSB: Special 2
  */
  case 0:
    ret |= map_trigger(game->pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
                       GTA2_CTRL_FORWARD);
    ret |= map_trigger(game->pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT,
                       GTA2_CTRL_BACKWARD);
    ret |= map_axis(game->pad, SDL_CONTROLLER_AXIS_LEFTX, GTA2_CTRL_RIGHT,
                    GTA2_CTRL_LEFT);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT,
                      GTA2_CTRL_WEAPON_PREV);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
                      GTA2_CTRL_WEAPON_NEXT);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_A, GTA2_CTRL_ATTACK);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_B,
                      GTA2_CTRL_JUMP_HANDBRAKE);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_X, GTA2_CTRL_ENTER_EXIT);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                      GTA2_CTRL_SPECIAL_1);
    ret |= map_button(game->pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
                      GTA2_CTRL_SPECIAL_2);
    break;

  case 1:
    // TODO
    break;

    // moar: TODO
  }
  return ret;
}
