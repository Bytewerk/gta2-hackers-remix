#pragma once
#include "../bg/background.h"
#include "../sfx/sfx.h"
#include "../sty/sty.h"
#include "actions.h"
#include "elements.h"
#include <SDL2/SDL.h>
#include <stdint.h>

// extends tk_el_t, el.actionfunc calls actionfunc
typedef struct tk_screen_t {
  tk_el_t el;
  tk_el_t *el_selected;
  struct tk_screen_t *back;
  void *actionfunc; // (tk, el, el_selected, action)
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
  uint32_t action_time[TK_ACTION_LAST_LIMITED - 1];
} tk_t;

tk_t *tk_init(sty_t *fsty, sfx_t *sfx, bg_t *bg, char *title);
void tk_frame(tk_t *tk, SDL_Event *event);
void tk_cleanup(tk_t *tk);

tk_screen_t *tk_screen(tk_t *tk, tk_screen_t *back, void *actionfunc);
void tk_screen_draw(tk_t *tk);
void tk_screen_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action);

#define TK_STACK_SCREEN(SCREEN, UICODE)                                        \
  {                                                                            \
    tk_el_t *TK_PARENT = &(SCREEN->el);                                        \
    TK_PARENT->type = STACK;                                                   \
    UICODE;                                                                    \
  }

void captions_draw_buttom_text(tk_t *tk);

void tk_action(tk_t *tk, SDL_Event *event);

// elements: common functions
tk_el_t *tk_el_attach(tk_el_t *el, tk_el_t *parent, tk_el_type_t type);
void tk_el_clean(tk_el_t *el);
void tk_el_geocalc(tk_t *tk, tk_el_t *el, char /*bool*/ down, char /*bool*/ up);

// extended elements
tk_el_t *tk_sprite(tk_t *tk, tk_el_t *parent, char sprite_id,
                   uint32_t argb_selected);
tk_el_t *tk_label(tk_t *tk, tk_el_t *parent, char *text);
