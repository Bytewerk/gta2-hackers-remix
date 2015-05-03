#pragma once
#include "../bg/background.h"
#include "../gxt/gxt.h"
#include "../sfx/sfx.h"
#include "../sty/sty.h"
#include "actions.h"
#include "elements.h"
#include <SDL2/SDL.h>
#include <stdint.h>

/*
        extends tk_el_t, el.actionfunc calls actionfunc

        define el_content_container, if the parent of the selectable
        entries is different from the first element defined (eg.
        if you have a title bar like on the high scores screen).
*/
typedef struct tk_screen_t {
  tk_el_t el;
  tk_el_t *el_selected;
  tk_el_t *el_content_container;
  struct tk_screen_t *back;
  void *actionfunc; // (tk, el, el_selected, action, key)
} tk_screen_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  tk_screen_t *screen_first;
  tk_screen_t *screen_active;
  gxt_t *gxt;
  sfx_t *sfx;
  sty_t *fsty;
  bg_t *bg;
  char quit;
  tk_el_t *exclusive_action_element;
  uint32_t action_time[TK_ACTION_LAST_LIMITED - 1];
  char redraw_needed;
  char debug_draw;
} tk_t;

tk_t *tk_init(gxt_t *gxt, sty_t *fsty, sfx_t *sfx, bg_t *bg, char *title);
void tk_frame(tk_t *tk, SDL_Event *event);
void tk_cleanup(tk_t *tk);

tk_screen_t *tk_screen(tk_t *tk, tk_screen_t *back, void *actionfunc);
void tk_screen_draw(tk_t *tk);
void tk_screen_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key);

#define TK_STACK_SCREEN(SCREEN, UICODE)                                        \
  {                                                                            \
    tk_el_t *TK_PARENT = &(SCREEN->el);                                        \
    TK_PARENT->type = STACK;                                                   \
    UICODE;                                                                    \
  }

void captions_draw_buttom_text(tk_t *tk);

void tk_action(tk_t *tk, SDL_Event *event);
void tk_actions_element(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                        tk_action_t action, SDL_Keycode key);

// elements: common functions
tk_el_t *tk_el_attach(tk_el_t *el, tk_el_t *parent, tk_el_type_t type);
void tk_el_clean(tk_t *tk, tk_el_t *el);
void tk_el_geocalc(tk_t *tk, tk_el_t *el, char /*bool*/ down, char /*bool*/ up);

// returns 1 if el_selection is el or a parent of el
char tk_is_selected_recursive(tk_el_t *el, tk_el_t *el_selected,
                              char recursing /*set to 0*/);

// event: invisible element with an actionfunc. Only use this, if you
// can't attach your actionfunc to another element (eg. if you would
// override a screen/control actionfunc)
tk_el_t *tk_event(tk_t *tk, tk_el_t *parent, void *actionfunc, void *userdata);

// extended elements
tk_el_t *tk_sprite(tk_t *tk, tk_el_t *parent, char sprite_id);
tk_el_t *tk_label(tk_t *tk, tk_el_t *parent, char *text, char font_id,
                  char font_id_selected);
