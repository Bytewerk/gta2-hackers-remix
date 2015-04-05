#include "sty/sty.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Syntax: %s sprite_id\n", argv[0]);
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  SDL_Window *window =
      SDL_CreateWindow("G2HR - sprite test", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0); // green
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  sty_t *fsty = sty_load("data/fstyle.sty");

  int sprite_id = atoi(argv[1]);
  SDL_Texture *sprite = sty_sprite(renderer, fsty, sprite_id);

  printf("sprite_id: %i\n", sprite_id);
  while (1) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;
    if (event.type == SDL_KEYDOWN) {
      SDL_DestroyTexture(sprite);
      sprite_id++;
      sprite = sty_sprite(renderer, fsty, sprite_id);
      printf("sprite_id: %i\n", sprite_id);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sprite, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}
