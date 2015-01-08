#include "menu.h"
#include "sty_parser.h"
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  sty_t *sty = sty_parser("fstyle.sty");
  menu_t *menu = menu_init();

  while (1) {
    SDL_Event event;
    menu_draw(menu);
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;
  }

  menu_cleanup(menu);
  SDL_Quit();
  sty_cleanup(sty);

  printf("Cleaned up!\n");
  return 0;
}
