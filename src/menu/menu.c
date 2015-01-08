#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

// call SDL_init before!
menu_t *menu_init() {
  menu_t *menu = malloc(sizeof(menu_t));

  menu->controller_count = 0;
  menu->screen = MAIN;

  // create the SDL window
  menu->window =
      SDL_CreateWindow("G2HR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN);
  if (!menu->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  menu->renderer = SDL_CreateRenderer(menu->window, -1, 0);
  SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 0); // black
  return menu;
}

void menu_cleanup(menu_t *menu) {
  SDL_DestroyRenderer(menu->renderer);
  SDL_DestroyWindow(menu->window);
  free(menu);
}

void menu_draw(menu_t *menu) {
  SDL_RenderClear(menu->renderer);

  // TODO!

  SDL_RenderPresent(menu->renderer);
}
