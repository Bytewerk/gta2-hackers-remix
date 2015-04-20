#include "../../sty/font.h"
#include "../../sty/sprites.h"
#include "../toolkit.h"
#include "controls.h"

typedef struct {
  char value_str[2]; // {value, '\0'}
  char min;
  char max;
  char min2;
  char max2;
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

  if ((!ud->max2 && value == ud->max) || (ud->max2 && value == ud->max2))
    tk_el_invisible(ud->right);
  else
    tk_el_visible(ud->right);
}

void circle_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action, SDL_Keycode key) {
  ud_circle_t *ud = (ud_circle_t *)el->userdata;
  if (el_selected != ud->container)
    return;

  char value = ud->value_str[0];

  // left arrow
  if ((action == TK_ACTION_LEFT || action == TK_ACTION_MOUSEDOWN) &&
      el == ud->left) {
    if (value > ud->min2 && ud->min2)
      ud->value_str[0]--;
    else if (value == ud->min2 && ud->min2)
      ud->value_str[0] = ud->max;
    else if (value > ud->min)
      ud->value_str[0]--;
  }

  // right arrow
  if ((action == TK_ACTION_RIGHT || action == TK_ACTION_MOUSEDOWN) &&
      el == ud->right) {
    if (value < ud->max)
      ud->value_str[0]++;
    else if (value == ud->max && ud->min2)
      ud->value_str[0] = ud->min2;
    else if (value < ud->max2 && ud->min2)
      ud->value_str[0]++;
  }
  circle_arrow_visibility(ud);
}

/*
        Define the range of possible characters with min, max, min2, max2.
        If min2 and max2 are not used, set them to 0.

        Example call:
                tk_ctrl_circle(tk, TK_PARENT, "CHOOSE A LETTER", NULL,
                        '1', '9', 'A', 'Z', '0', NULL)
*/
tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char min2,
                        char max2, char value, void *actionfunc) {
  ud_circle_t *ud = calloc(1, sizeof(ud_circle_t));
  ud->actionfunc = actionfunc;
  ud->min = min;
  ud->max = max;
  ud->min2 = min2;
  ud->max2 = max2;
  ud->value_str[0] = value;
  ud->value_str[1] = '\0';

  TK_STACK(
      ud->container = TK_PARENT; ud->container->userdata = (void *)ud;
      ud->container->bg_mashup = bg_mashup; tk_el_selectable(ud->container);

      ud->button = tk_ctrl_button(tk, TK_PARENT, text, NULL, NULL,
                                  (void *)circle_actionfunc);
      ud->button->userdata = ud;

      TK_FLOW(tk_el_padding(TK_PARENT, 78, 2, 0, 0);

              ud->left = tk_ctrl_arrow(tk, TK_PARENT, 1,
                                       (void *)circle_actionfunc, (void *)ud);

              // circle sprite
              ud->circle_sprite = tk_sprite(tk, TK_PARENT, GTA2_SPRITE_CIRCLE);
              ud->circle_sprite->width = 32; ud->circle_sprite->height = 32;

              // circle text
              ud->circle_text = tk_label(tk, TK_PARENT, ud->value_str);
              ud->circle_text->font_id = GTA2_FONT_FSTYLE_WHITE_RED_NORMAL;
              tk_el_padding(ud->circle_text, -23, 4, 0, 0);
              tk_el_width(ud->circle_text, -1 * ud->circle_text->padding_left);

              ud->right = tk_ctrl_arrow(tk, TK_PARENT, 0,
                                        (void *)circle_actionfunc, (void *)ud);

              tk_el_padding(ud->left, 18, 8, 18, 0);
              tk_el_padding(ud->right, 18, 8, 18, 0);

              ););

  circle_arrow_visibility(ud);
  return ud->container;
}
