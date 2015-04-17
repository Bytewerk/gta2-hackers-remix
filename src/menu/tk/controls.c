#include "controls.h"
#include "../sty/font.h"
#include "../sty/sprites.h"
#include "toolkit.h"

//
// BUTTON
//

typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
} ud_button_t;

void button_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action) {
  if (action != TK_ACTION_ENTER || el != el_selected)
    return;
  // TODO: or action is mouse click...

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
  label->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;
  label->font_id_selected = GTA2_FONT_FSTYLE_RED_BLACK_NORMAL;

  return label;
}

//
// ARROW
//

tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc, void *userdata) {
  tk_el_t *sprite =
      tk_sprite(tk, TK_PARENT, is_left ? GTA2_SPRITE_ARROW_LEFT_RED
                                       : GTA2_SPRITE_ARROW_RIGHT_RED);
  sprite->actionfunc = actionfunc;
  sprite->userdata = userdata;

  // only display, when selected
  sprite->argb_normal = 0x00ffffff;
  sprite->argb_selected = 0xffffffff;
  return sprite;
}

//
// CIRCLE
//

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
  if (el == ud->left && value - 1 >= ud->min &&
      (action == TK_ACTION_LEFT || action == TK_ACTION_MOUSEDOWN))
    ud->value_str[0]--;

  if (el == ud->right && value + 1 <= ud->max &&
      (action == TK_ACTION_RIGHT || action == TK_ACTION_MOUSEDOWN))
    ud->value_str[0]++;

  circle_arrow_visibility(ud);
}

tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char value,
                        void *actionfunc) {
  ud_circle_t *ud = calloc(1, sizeof(ud_circle_t));
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

      TK_FLOW(tk_el_padding(TK_PARENT, 78, 2, 0, 0);

              ud->left = tk_ctrl_arrow(tk, TK_PARENT, 1,
                                       (void *)circle_actionfunc, (void *)ud);

              // circle sprite
              ud->circle_sprite = tk_sprite(tk, TK_PARENT, GTA2_SPRITE_CIRCLE);
              ud->circle_sprite->width = 32; ud->circle_sprite->height = 32;

              // circle text
              ud->circle_text = tk_label(tk, TK_PARENT, ud->value_str);
              ud->circle_text->font_id = GTA2_FONT_FSTYLE_WHITE_RED_NORMAL;
              tk_el_padding(ud->circle_text, -23, 0, 4, 0);
              tk_el_width(ud->circle_text, -1 * ud->circle_text->padding_left);

              ud->right = tk_ctrl_arrow(tk, TK_PARENT, 0,
                                        (void *)circle_actionfunc, (void *)ud);

              tk_el_padding(ud->left, 18, 8, 18, 0);
              tk_el_padding(ud->right, 18, 8, 18, 0);

              ););

  circle_arrow_visibility(ud);
  return ud->container;
}

//
// ARROWTEXT
//

typedef struct {
  uint16_t entry_count;
  uint16_t entry_selected;
  char entry_length;
  char **entries;
  void *actionfunc;
  char editing_disabled;
  char *bottom_text_high;
  char *bottom_text_low;
  char *bottom_text_high_editing;
  char *bottom_text_low_editing;
  tk_el_t *container;
  tk_el_t *left;
  tk_el_t *text;
  tk_el_t *underscore;
  tk_el_t *right;
} ud_arrowtext_t;

void arrowtext_style(tk_t *tk, ud_arrowtext_t *ud) {
  char is_editing = (tk->exclusive_action_element == ud->container);

  // left arrow visibility
  if (is_editing || ud->entry_selected == 0)
    tk_el_invisible(ud->left);
  else
    tk_el_visible(ud->left);

  // right arrow visibility
  if (is_editing || ud->entry_selected == ud->entry_count - 1)
    tk_el_invisible(ud->right);
  else
    tk_el_visible(ud->right);

  // underscore visibility
  if (is_editing)
    tk_el_visible(ud->underscore);
  else
    tk_el_invisible(ud->underscore);

  // bottom text high
  ud->container->bottom_text_high =
      is_editing ? ud->bottom_text_high_editing : ud->bottom_text_high;

  // bottom text low
  ud->container->bottom_text_low =
      is_editing ? ud->bottom_text_low_editing : ud->bottom_text_low;

  // label text
  if (ud->text->text != ud->entries[ud->entry_selected]) {
    ud->text->text = ud->entries[ud->entry_selected];
    tk_el_geocalc(tk, ud->text, 0, 1);
  }
}

void arrowtext_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action) {
  ud_arrowtext_t *ud = (ud_arrowtext_t *)el->userdata;
  char is_editing = (tk->exclusive_action_element == ud->container);

  if (el == ud->container) {
    if (is_editing) {
      // FIXME: restore name on ESC
      if (action == TK_ACTION_ENTER || action == TK_ACTION_ESC)
        tk->exclusive_action_element = NULL;
      if (action == TK_ACTION_BACKSPACE)
        printf("BACKSPACE :O!\n");
    } else // not editing
    {
      if (!ud->editing_disabled && action == TK_ACTION_ENTER)
        tk->exclusive_action_element = ud->container;
    }
  }

  if (!is_editing) {
    // right arrow
    if (el == ud->right && ud->entry_selected < ud->entry_count - 1 &&
        (action == TK_ACTION_RIGHT || action == TK_ACTION_MOUSEDOWN))
      ud->entry_selected++;

    // left arrow
    if (el == ud->left && ud->entry_selected > 0 &&
        (action == TK_ACTION_LEFT || action == TK_ACTION_MOUSEDOWN))
      ud->entry_selected--;
  }
  arrowtext_style(tk, el->userdata);
}

tk_el_t *tk_ctrl_arrowtext(tk_t *tk, tk_el_t *TK_PARENT, bg_mashup_t *bg_mashup,
                           char **entries, uint16_t entry_count,
                           uint16_t entry_selected, char entry_length,
                           void *actionfunc, char editing_disabled,
                           char *bottom_text_high, char *bottom_text_low,
                           char *bottom_text_high_editing,
                           char *bottom_text_low_editing) {
  ud_arrowtext_t *ud = calloc(1, sizeof(ud_arrowtext_t));
  ud->actionfunc = actionfunc;
  ud->entries = entries;
  ud->entry_length = entry_length;
  ud->entry_count = entry_count;
  ud->entry_selected = entry_selected;
  ud->actionfunc = actionfunc;
  ud->editing_disabled = editing_disabled;

  ud->bottom_text_high = bottom_text_high;
  ud->bottom_text_low = bottom_text_low;
  ud->bottom_text_high_editing = bottom_text_high_editing;
  ud->bottom_text_low_editing = bottom_text_low_editing;

  TK_FLOW(ud->container = TK_PARENT; ud->container->bg_mashup = bg_mashup;
          ud->container->actionfunc = (void *)arrowtext_actionfunc;
          ud->container->userdata = ud;

          ud->left = tk_ctrl_arrow(tk, TK_PARENT, 1,
                                   (void *)arrowtext_actionfunc, (void *)ud);

          ud->text = tk_label(tk, TK_PARENT, entries[entry_selected]);
          ud->text->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;
          ud->text->font_id_selected = GTA2_FONT_FSTYLE_RED_BLACK_NORMAL;

          ud->underscore = tk_label(tk, TK_PARENT, "_");
          ud->underscore->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;

          ud->right = tk_ctrl_arrow(tk, TK_PARENT, 0,
                                    (void *)arrowtext_actionfunc, (void *)ud);

          tk_el_padding(ud->left, 5, 4, 5, 0);
          tk_el_padding(ud->right, 5 - ud->underscore->width, 4, 5, 0);
          tk_el_padding(TK_PARENT,
                        (-1) * (ud->left->width + ud->left->padding_left +
                                ud->left->padding_right),
                        0, 0, 0););

  arrowtext_style(tk, ud);
  return ud->container;
}
