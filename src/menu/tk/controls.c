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

tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen,
                        void *onclick_func) {
  tk_el_t *label = tk_label(tk, TK_PARENT, text);

  ud_button_t *ud = malloc(sizeof(ud_button_t));
  ud->onclick_func = onclick_func;
  ud->onclick_screen = onclick_screen;
  label->bg_mashup = bg_mashup;
  label->userdata = (void *)ud;
  label->actionfunc = (void *)button_actionfunc;

  return label;
}

// ARROW
tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc) {
  tk_el_t *sprite =
      tk_sprite(tk, TK_PARENT, is_left ? GTA2_SPRITE_ARROW_LEFT_WHITE
                                       : GTA2_SPRITE_ARROW_RIGHT_WHITE,
                0xffff0000);
  sprite->actionfunc = actionfunc;
  sprite->width = 16;
  sprite->height = 16;
  return sprite;
}

// CIRCLE
typedef struct {
  char value_str[2]; // value, \0
  char min;
  char max;
  tk_el_t *container;
  tk_el_t *button;
  tk_el_t *left;
  tk_el_t *right;
  tk_el_t *circle_sprite;
  tk_el_t *circle_text;
  void *actionfunc;
} ud_circle_t;

void circle_arrow_visibility(ud_circle_t *ud) {
  char value = ud->value_str[0];
  if (value == ud->min)
    tk_el_invisible(ud->left);
  else
    tk_el_visible(ud->left);

  if (value == ud->max)
    tk_el_invisible(ud->right);
  else
    tk_el_visible(ud->right);
}

void circle_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action) {
  ud_circle_t *ud = (ud_circle_t *)el->userdata;
  if (el_selected != ud->container)
    return;

  char value = ud->value_str[0];
  if (action == TK_ACTION_LEFT && value - 1 >= ud->min)
    ud->value_str[0]--;

  if (action == TK_ACTION_RIGHT && value + 1 <= ud->max)
    ud->value_str[0]++;

  circle_arrow_visibility(ud);
}

tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char value,
                        void *actionfunc) {
  ud_circle_t *ud = malloc(sizeof(ud_circle_t));
  ud->actionfunc = actionfunc;
  ud->min = min;
  ud->max = max;
  ud->value_str[0] = value;
  ud->value_str[1] = '\0';

  TK_STACK(
      ud->container = TK_PARENT; TK_PARENT->userdata = (void *)ud;
      TK_PARENT->bg_mashup = bg_mashup;

      ud->button = tk_ctrl_button(tk, TK_PARENT, text, NULL, NULL,
                                  (void *)circle_actionfunc);
      ud->button->userdata = ud;

      TK_FLOW(
          tk_el_padding(TK_PARENT, 100, 0, 0, 0);

          ud->left = tk_ctrl_arrow(tk, TK_PARENT, 1, (void *)circle_actionfunc);
          ud->left->userdata = ud;

          // circle sprite
          ud->circle_sprite = tk_sprite(tk, TK_PARENT, GTA2_SPRITE_CIRCLE, 0);
          ud->circle_sprite->width = 32; ud->circle_sprite->height = 32;

          // circle text
          ud->circle_text = tk_label(tk, TK_PARENT, ud->value_str);
          tk_el_padding(ud->circle_text, -23, 0, 4, 0);
          tk_el_width(ud->circle_text, -1 * ud->circle_text->padding_left);

          ud->right =
              tk_ctrl_arrow(tk, TK_PARENT, 0, (void *)circle_actionfunc);
          ud->right->userdata = ud;

          tk_el_padding(ud->left, 10, 8, 10, 0);
          tk_el_padding(ud->right, 10, 8, 10, 0);

          ););

  circle_arrow_visibility(ud);
  return ud->container;
}
