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
#define ARROW_PADDING_TOP 5
tk_el_t *tk_ctrl_arrow(tk_el_t *TK_PARENT, char is_left, void *actionfunc) {
  tk_el_t *sprite =
      tk_sprite(TK_PARENT, is_left ? GTA2_SPRITE_ARROW_LEFT_WHITE
                                   : GTA2_SPRITE_ARROW_RIGHT_WHITE,
                0xffff0000);
  sprite->actionfunc = actionfunc;
  sprite->width = 16;
  sprite->height = 16;
  tk_el_padding(sprite, (is_left ? -16 : 0), ARROW_PADDING_TOP,
                (is_left ? 0 : 16), 0);

  return sprite;
}

// CIRCLE
void circle_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action) {}
typedef struct {
  char value;
  char min;
  char max;
  tk_el_t *button;
  tk_el_t *left;
  tk_el_t *right;
  tk_el_t *circle_sprite;
  tk_el_t *circle_text;
  void *actionfunc;
} ud_circle_t;

tk_el_t *tk_ctrl_circle(tk_el_t *TK_PARENT, char *text, bg_mashup_t *bg_mashup,
                        char min, char max, char value, void *actionfunc) {
  ud_circle_t *ud = malloc(sizeof(ud_circle_t));
  ud->actionfunc = actionfunc;
  ud->min = min;
  ud->max = max;
  ud->value = value;

  tk_el_t *ret;
  TK_STACK(
      ret = TK_PARENT; TK_PARENT->userdata = (void *)ud;
      TK_PARENT->bg_mashup = bg_mashup;

      tk_ctrl_button(TK_PARENT, text, NULL, NULL, (void *)circle_actionfunc);

      TK_FLOW(tk_el_padding(TK_PARENT, 130, 0, 0, 0);

              ud->left = tk_ctrl_arrow(TK_PARENT, 1, (void *)circle_actionfunc);

              // circle sprite
              ud->circle_sprite = tk_sprite(TK_PARENT, GTA2_SPRITE_CIRCLE, 0);
              ud->circle_sprite->width = 32; ud->circle_sprite->height = 32;

              // circle text (TODO)

              ud->right =
                  tk_ctrl_arrow(TK_PARENT, 0, (void *)circle_actionfunc););

      );

  return ret;
}
