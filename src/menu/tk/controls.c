#include "controls.h"
#include "../sty/sprites.h"
#include "toolkit.h"

// BUTTON
typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
} ud_button_t;

void button_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action) {
  if (action != TK_ACTION_ENTER || el != el_selected)
    return;

  ud_button_t *ud = (ud_button_t *)el->userdata;
  // if(ud->onclick_func) - TODO

  if (ud->onclick_screen)
    tk->screen_active = ud->onclick_screen;
}

tk_el_t *tk_ctrl_button(tk_el_t *TK_PARENT, char *text, bg_mashup_t *bg_mashup,
                        tk_screen_t *onclick_screen, void *onclick_func) {
  tk_el_t *label = tk_label(TK_PARENT, text);

  ud_button_t *ud = malloc(sizeof(ud_button_t));
  ud->onclick_func = onclick_func;
  ud->onclick_screen = onclick_screen;
  label->bg_mashup = bg_mashup;
  label->userdata = (void *)ud;
  label->actionfunc = (void *)button_actionfunc;

  return label;
}

// ARROW
tk_el_t *tk_ctrl_arrow(tk_el_t *TK_PARENT, char is_left, void *actionfunc) {
  tk_el_t *sprite =
      tk_sprite(TK_PARENT, is_left ? GTA2_SPRITE_ARROW_LEFT_WHITE
                                   : GTA2_SPRITE_ARROW_RIGHT_WHITE,
                0xffff0000);
  sprite->actionfunc = actionfunc;
  sprite->width = 16;
  sprite->height = 16;
  tk_el_padding(sprite, is_left ? -16 : 0, 0, is_left ? 0 : 16, 0);

  return sprite;
}
