#include "pads.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());

  printf("running init...\n");
  pads_t *pads = pads_init(1);

  while (1) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT)
      break;
    pads_frame(pads, &e, 1);
  }

  printf("cleaning up...\n");
  pads_cleanup(pads);

  SDL_Quit();
}
