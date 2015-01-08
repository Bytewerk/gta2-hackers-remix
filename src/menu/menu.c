#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

// When implementing font drawing, this might be handy:
// http://www.programmersranch.com/2014/02/sdl2-pixel-drawing.html
// also blackphoenix (the creator of openGTA2) has created
// images for missing characters, which will probably be useful

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

// Explanation of surface -> texture -> renderer:
// http://www.programmersranch.com/2013/11/sdl2-displaying-image-in-window.html

void menu_draw(menu_t *menu) {
  SDL_RenderClear(menu->renderer);

  // TODO: cache all menu images first!
  SDL_Surface *left = IMG_Load("1_Play.tga");
  SDL_Surface *right = IMG_Load("1.tga");

  SDL_Texture *l_tex = SDL_CreateTextureFromSurface(menu->renderer, left);
  SDL_Texture *r_tex = SDL_CreateTextureFromSurface(menu->renderer, right);

  SDL_Rect dstrect_l = {0, 0, 320, 480};
  SDL_RenderCopy(menu->renderer, l_tex, NULL, &dstrect_l);

  SDL_Rect dstrect_r = {320, 0, 320, 480};
  SDL_RenderCopy(menu->renderer, r_tex, NULL, &dstrect_r);

  SDL_RenderPresent(menu->renderer);

  SDL_DestroyTexture(l_tex);
  SDL_DestroyTexture(r_tex);
  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
}
