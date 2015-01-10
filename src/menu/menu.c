#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

// When implementing font drawing, this might be handy:
// http://www.programmersranch.com/2014/02/sdl2-pixel-drawing.html
// also blackphoenix (the creator of openGTA2) has created
// images for missing characters, which will probably be useful

// menu->renderer must be initialized before.
// The menu should be started in the GTA2 path
// as working dir.
void menu_tga_load(menu_t *menu, const char *name) {
  menu_tga_texture_t *tga_texture = malloc(sizeof(menu_tga_texture_t));
  tga_texture->name = name;
  tga_texture->next = NULL;

  char fullpath[100];
  snprintf(fullpath, sizeof(fullpath), "data/frontend/%s.tga", name);

  printf("loading %s...\n", fullpath);

  SDL_Surface *surface = IMG_Load(fullpath);
  if (!surface)
    exit(printf("Failed to load '%s'!\n", fullpath));

  // Create a texture from the surface and free it afterwards
  tga_texture->tex = SDL_CreateTextureFromSurface(menu->renderer, surface);
  tga_texture->width = surface->w;
  tga_texture->height = surface->h;
  SDL_FreeSurface(surface);

  // append the new texture to the list
  if (menu->tga_textures) {
    menu_tga_texture_t *list_pos = menu->tga_textures;
    while (list_pos->next)
      list_pos = list_pos->next;
    list_pos->next = tga_texture;
  } else
    menu->tga_textures = tga_texture;
}

menu_tga_texture_t *menu_tga_get(menu_t *menu, const char *name) {
  menu_tga_texture_t *list_pos = menu->tga_textures;
  while (list_pos) {
    if (list_pos->name == name)
      return list_pos;
    list_pos = list_pos->next;
  }

  printf("ERROR: tried to access non-cached file: %s.tga\n", name);
  exit(1);
}

// call SDL_init before
menu_t *menu_init() {
  menu_t *menu = malloc(sizeof(menu_t));

  menu->controller_count = 0;
  menu->screen = MAIN;
  menu->tga_textures = NULL;

  // create the SDL window
  menu->window =
      SDL_CreateWindow("G2HR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN);
  if (!menu->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  menu->renderer = SDL_CreateRenderer(menu->window, -1, 0);
  SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 0); // black

  // cache all GTA2 menu TGAs as textures
  const char *tga_files[] = {"1", "1_Play", "1_Options",
                             "1_Quit"}; // more: todo!
  for (int i = 0; i < (sizeof(tga_files) / sizeof(char *)); i++)
    menu_tga_load(menu, tga_files[i]);

  return menu;
}

void menu_cleanup(menu_t *menu) {
  // free all tga textures
  menu_tga_texture_t *list_pos = menu->tga_textures;
  while (list_pos && list_pos->next) {
    menu_tga_texture_t *delete_me = list_pos;
    list_pos = delete_me->next;
    SDL_DestroyTexture(delete_me->tex);
    free(delete_me);
  }

  SDL_DestroyRenderer(menu->renderer);
  SDL_DestroyWindow(menu->window);
  free(menu);
}

// Explanation of surface -> texture -> renderer:
// http://www.programmersranch.com/2013/11/sdl2-displaying-image-in-window.html

void menu_draw(menu_t *menu) {
  SDL_RenderClear(menu->renderer);

  SDL_Texture *left = menu_tga_get(menu, "1_Play")->tex;
  SDL_Texture *right = menu_tga_get(menu, "1")->tex;

  SDL_Rect dstrect_l = {0, 0, 320, 480};
  SDL_Rect dstrect_r = {320, 0, 320, 480};

  SDL_RenderCopy(menu->renderer, left, NULL, &dstrect_l);
  SDL_RenderCopy(menu->renderer, right, NULL, &dstrect_r);

  SDL_RenderPresent(menu->renderer);
}
