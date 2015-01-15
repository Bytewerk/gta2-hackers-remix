#pragma once
#include "sty_parser.h"
#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
  // should be set to NULL, if not used
  // the textured should only be free'd by
  // menu_cleanup()
  menu_tga_texture_t *bg_left;
  menu_tga_texture_t *bg_right;
  menu_tga_texture_t *bg_full;

  // TODO: bg_overlay for custom backgrounds,
  // that are transparent in the area of the GTA2
  // logo
} menu_background_t;

typedef enum {
  BUTTON,
  TEXT_FIELD,
  RANGE_SELECTOR,
  BOOLEAN, // CHEAT xy: ENABLED / DISABLED
           // ...
} menu_element_type_t;

typedef struct menu_element_t {
  struct menu_element_t *next;

  char *title;
  menu_element_type_t type;
  void *type_specific_data;

  // set this to null if it should use the menu_screen
  // background instead
  menu_background_t *bg;
} menu_element_t;

typedef struct {
  menu_element_t *first_element;
  menu_element_t *selected_element;

  // set to NULL if not used (black background)
  // the bg can also be set in the menu elements
  menu_background_t *bg;
} menu_screen_t;

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
  sty_t *fstyle;

  unsigned char controller_count;
  menu_screen_t *screen;
} menu_t;

menu_t *menu_init();
void menu_draw(menu_t *menu);
void menu_cleanup(menu_t *menu);
