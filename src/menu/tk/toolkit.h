#pragma once
#include "../bg/background.h"
#include "../sfx/sfx.h"
#include "../sty/sty.h"
#include "elements.h"
#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct tk_screen_t // extends tk_el_t
{
  tk_el_t el;
  tk_el_t *selected_el;
  tk_el_t *first_el;
  struct tk_screen_t *back;
} tk_screen_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  tk_screen_t *screen_first;
  tk_screen_t *screen_active;
  sfx_t *sfx;
  sty_t *fsty;
  bg_t *bg;
  char quit;
} tk_t;

tk_t *tk_init(sty_t *fsty, sfx_t *sfx, bg_t *bg, char *title);
void tk_frame(tk_t *tk, SDL_Event *event);
void tk_cleanup(tk_t *tk);
