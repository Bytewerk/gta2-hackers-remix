#include "sty/sty.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  SDL_Window *window =
      SDL_CreateWindow("G2HR - sprite test", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // black

  sty_t *fsty = sty_load("data/fstyle.sty");

  // DEBUG: sprite 4 is a red circle with a white
  // border and transparend background.
  sty_sprite(fsty, 4);

  while (1) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    SDL_RenderClear(renderer);

    // TODO: draw sprites here for testing!

    SDL_RenderPresent(renderer);
  }
}
