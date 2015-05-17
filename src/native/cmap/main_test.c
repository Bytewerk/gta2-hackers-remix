#include "cmap.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  SDL_Init(SDL_INIT_GAMECONTROLLER);
  cmap_t *cmap = cmap_init();

  cmap_state_t *state = &(cmap->walking);

  for (SDL_GameControllerButton i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
    uint16_t new = cmap_action_to_movement_bitmask(state->buttons[i]);

    printf("button %s mapped to action: %i => bitmask: %i\n",
           SDL_GameControllerGetStringForButton(i), state->buttons[i], new);
  }

  printf("cleaning up...\n");
  cmap_cleanup(cmap);
  SDL_Quit();
}
