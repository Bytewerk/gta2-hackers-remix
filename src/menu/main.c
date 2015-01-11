#include "fonts.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  menu_t *menu = menu_init();

  letter_draw(menu, 2, 0, 0, 'a');

  while (1) {
    SDL_Event event;
    menu_draw(menu);
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;
  }

  menu_cleanup(menu);
  SDL_Quit();

  printf("Cleaned up!\n");
  return 0;
}
