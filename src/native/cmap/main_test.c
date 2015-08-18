#include "cmap.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  SDL_Init(SDL_INIT_GAMECONTROLLER);
  cmap_t *cmap = cmap_init();

  for (int is_driving = 0; is_driving < 2; is_driving++) {
    cmap_state_t *state = is_driving ? &(cmap->driving) : &(cmap->walking);

    printf("------ STATE: %s --------\n", is_driving ? "DRIVING" : "WALKING");

    for (SDL_GameControllerButton i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
      uint16_t new = cmap_action_to_movement_bitmask(state->buttons[i]);

      printf("button %s mapped to action: %i => bitmask: %i\n",
             SDL_GameControllerGetStringForButton(i), state->buttons[i], new);
    }

    for (int is_rightstick = 0; is_rightstick < 2; is_rightstick++) {
      cmap_deadzone_t d =
          is_rightstick ? state->dead_rightstick : state->dead_leftstick;

      printf("%s - %sSTICK DEADZONES\n", is_driving ? "DRIVING" : "WALKING",
             is_rightstick ? "RIGHT" : "LEFT");

      printf("\tleft:  %7i\n", d.left);
      printf("\tup:    %7i\n", d.up);
      printf("\tright: %7i\n", d.right);
      printf("\tdown:  %7i\n", d.down);
    }
  }
  printf("--------------\n");

  printf("cleaning up...\n");
  cmap_cleanup(cmap);
  SDL_Quit();
}
