#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

typedef enum { MAIN, SPLITSCREEN_SETUP, SETTINGS, CREDITS } screen_t;

typedef struct menu_tga_texture_t {
  struct menu_tga_texture_t *next;
  const char *name;
  SDL_Texture *tex;
  uint16_t width;
  uint16_t height;
} menu_tga_texture_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  menu_tga_texture_t *tga_textures;

  unsigned char controller_count;
  screen_t screen;
} menu_t;

menu_t *menu_init();
void menu_draw(menu_t *menu);
void menu_cleanup(menu_t *menu);
