#include "toolkit/toolkit.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk_t *tk = tk_init("G2HR");
  tk_init_gta2_background(tk, "1");
  tk_init_gta2_background(tk, "1_Play");

  tk_screen_t *main = tk_screen_create(tk, NULL, "1_Play", "1");

  tk->screen = main;

  while (1) {
    SDL_Event event;
    tk_frame(tk);
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;
  }

  tk_cleanup(tk);
  SDL_Quit();

  printf("Cleaned up!\n");
  return 0;
}
