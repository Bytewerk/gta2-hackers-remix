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
  SDL_Texture *sprite = sty_sprite(renderer, fsty, 4);
  SDL_Rect dest_rect = {0, 0, 640, 480};

  while (1) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sprite, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
  }
}
