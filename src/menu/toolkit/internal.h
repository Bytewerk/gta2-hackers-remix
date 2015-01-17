#pragma once
#include "../sty_loader/sty_parser.h"
#include <SDL2/SDL.h>
#include <stdint.h>

// textures must only be free'd by tk_cleanup()!
typedef struct tk_texture_t {
  struct tk_texture_t *next;
  const char *name;
  SDL_Texture *texture;
  uint16_t width;
  uint16_t height;
} tk_texture_t;

typedef struct {
  tk_texture_t *full;
  tk_texture_t *left;
  tk_texture_t *right;
} tk_background_t;

struct tk_screen_t;
typedef struct tk_control_t {
  struct tk_control_t *next;
  const char *title;
  char type;
  void *data;
  tk_background_t *bg; // NULL: use screen bg
  struct tk_screen_t *onclick_screen;
} tk_control_t;

typedef struct tk_screen_t {
  struct tk_screen_t *back;
  tk_control_t *first_control;
  tk_control_t *selected_control;
  tk_background_t *bg; // NULL: control bg || black
  void *ui_data;
  void *event_func; // func(tk_t* tk, ui_t* ui)
} tk_screen_t;

typedef struct {
  sty_t *fstyle;
  SDL_Window *window;
  SDL_Renderer *renderer;
  tk_texture_t *textures;
  tk_screen_t *screen;
} tk_t;

void tk_screen_draw(tk_t *tk);
tk_texture_t *tk_texture_get(tk_t *tk, const char *name);

tk_background_t *tk_create_background(tk_t *tk, const char *full,
                                      const char *left, const char *right);

tk_control_t *tk_control_cleanup(tk_control_t *ctrl);

void tk_control_up(tk_screen_t *screen);
void tk_control_down(tk_screen_t *screen);
void tk_screen_back(tk_t *tk);
void tk_control_enter(tk_t *tk);
