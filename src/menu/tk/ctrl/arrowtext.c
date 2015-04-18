#include "../../sty/font.h"
#include "../toolkit.h"
#include "controls.h"
#include <string.h>

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

  // label text (comparing pointers here!)
  if (!is_editing && ud->text->text != ud->entries[ud->entry_selected]) {
    ud->text->text = ud->entries[ud->entry_selected];
    tk_el_geocalc(tk, ud->text, 0, 1);
  }
}

void arrowtext_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key) {
  ud_arrowtext_t *ud = (ud_arrowtext_t *)el->userdata;
  char is_editing = (tk->exclusive_action_element == ud->container);

  if (el == ud->container) {
    if (is_editing) {
      if (action == TK_ACTION_ESC)
        tk->exclusive_action_element = NULL;
      if (action == TK_ACTION_ENTER) {
        strncpy(ud->entries[ud->entry_selected], ud->text->text,
                ud->entry_length + 1);
        tk->exclusive_action_element = NULL;
      }
      if (action == TK_ACTION_BACKSPACE) {
        char *text = ud->text->text;
        int length = strlen(text);
        if (!length)
          return;
        text[length - 1] = '\0';

        tk_el_geocalc(tk, ud->text, 0, 1);
      }
      if (action == TK_ACTION_TYPING) {
        char *text = ud->text->text;
        int length = strlen(text);
        if (length >= ud->entry_length)
          return;
        char letter = 0x00;

        // http://wiki.libsdl.org/SDL_Keycode
        if (key >= SDLK_SPACE && key <= SDLK_UNDERSCORE)
          letter = key - SDLK_SPACE + ' ';
        if (key >= SDLK_a && key <= SDLK_z)
          letter = key - SDLK_a + 'A';

        if (letter) {
          text[length] = letter;
          text[length + 1] = '\0';
        }

        tk_el_geocalc(tk, ud->text, 0, 1);
      }
    } else // not editing
    {
      if (!ud->editing_disabled && action == TK_ACTION_ENTER) {
        tk->exclusive_action_element = ud->container;

        // copy the string to the text element,
        // so we can restore it if necessary
        ud->text->text = malloc(ud->entry_length + 1);
        strncpy(ud->text->text, ud->entries[ud->entry_selected],
                ud->entry_length + 1);
      }
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
          ud->container->userdata = ud; tk_el_selectable(ud->container);

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
