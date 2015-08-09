#include "../../sty/font.h"
#include "../toolkit.h"
#include "controls.h"
#include <string.h>

void tk_ctrl_arrowtext_disabled(tk_el_t *ctrl) {
  ud_arrowtext_t *ud = ((ud_arrowtext_t *)(ctrl->userdata));
  tk_el_disabled(ctrl);
  if (ud->text_pre)
    tk_el_disabled(ud->text_pre);
  tk_el_disabled(ud->text);
  if (ud->text_suf)
    tk_el_disabled(ud->text_suf);
}

void tk_ctrl_arrowtext_enabled(tk_el_t *ctrl) {
  ud_arrowtext_t *ud = ((ud_arrowtext_t *)(ctrl->userdata));
  tk_el_enabled(ctrl);
  if (ud->text_pre)
    tk_el_enabled(ud->text_pre);
  tk_el_enabled(ud->text);
  if (ud->text_suf)
    tk_el_enabled(ud->text_suf);
}

void arrowtext_style(tk_t *tk, ud_arrowtext_t *ud) {
  char is_editing = (tk->exclusive_action_element == ud->container);

  // left arrow visibility
  if (is_editing || (ud->entry_selected == 0 && ud->entry_count != 2))
    tk_el_invisible(ud->left);
  else
    tk_el_visible(ud->left);

  // right arrow visibility
  if (is_editing ||
      (ud->entry_selected == ud->entry_count - 1 && ud->entry_count != 2))
    tk_el_invisible(ud->right);
  else
    tk_el_visible(ud->right);

  // underscore visibility
  if (ud->editing_maxlen) {
    if (is_editing)
      tk_el_visible(ud->underscore);
    else
      tk_el_invisible(ud->underscore);
  }

  // bottom text high
  ud->container->bottom_text_high =
      is_editing ? ud->bottom_text_high_editing : ud->bottom_text_high;

  // bottom text low
  ud->container->bottom_text_low =
      is_editing ? ud->bottom_text_low_editing : ud->bottom_text_low;

  // label text (comparing string pointers here!)
  if (!is_editing && ud->text->text != ud->entries[ud->entry_selected]) {
    ud->text->text = ud->entries[ud->entry_selected];
    tk_el_geocalc(tk, ud->text, 0, 1);
  }
}

void arrowtext_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key) {
  ud_arrowtext_t *ud = (ud_arrowtext_t *)el->userdata;
  char is_editing = (tk->exclusive_action_element == ud->container);

  if (action == TK_ACTION_CLEANUP) {
    if (ud->free_bool_default_entries && ud->entries) {
      free(ud->entries);
      ud->entries = NULL;
    }
    return;
  }

  if (el == ud->container) {
    if (action == TK_ACTION_FRAMETIME && ud->editing_maxlen) {
      ud->underscore_frame_count++;
      if (ud->underscore_frame_count == UNDERSCORE_BLINK_FRAMES - 1) {
        ud->underscore_frame_count = 0;
        ud->underscore->argb_selected =
            (ud->underscore->argb_selected == 0x00ffffff) ? 0xffffffff
                                                          : 0x00ffffff;
        tk->redraw_needed = 1;
      }
    }

    if (is_editing) {
      if (action == TK_ACTION_ESC)
        tk->exclusive_action_element = NULL;
      if (action == TK_ACTION_ENTER) {
        strncpy(ud->entries[ud->entry_selected], ud->text->text,
                ud->editing_maxlen + 1);
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
        if (length >= ud->editing_maxlen)
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
      if (action == TK_ACTION_ENTER &&
          tk_is_selected_recursive(el, el_selected, 0)) {
        if (ud->editing_maxlen) {
          tk->exclusive_action_element = ud->container;

          // copy the string to the text element,
          // so we can restore it if necessary
          ud->text->text = malloc(ud->editing_maxlen + 1);
          strncpy(ud->text->text, ud->entries[ud->entry_selected],
                  ud->editing_maxlen + 1);
        }

        // booleans: toggle!
        else if (ud->entry_count == 2)
          ud->entry_selected = !ud->entry_selected;
      }
    }
  }

  if (!is_editing && tk_is_selected_recursive(el, el_selected, 0)) {
    // right arrow
    if (el == ud->right &&
        (action == TK_ACTION_RIGHT || action == TK_ACTION_MOUSEDOWN)) {
      if (ud->entry_selected < ud->entry_count - 1)
        ud->entry_selected++;
      else if (ud->entry_count == 2) // toggle
        ud->entry_selected = 0;

      sfx_play_sample(tk->sfx, SFX_FSTYLE_LEFT_RIGHT);
    }

    // left arrow
    if (el == ud->left &&
        (action == TK_ACTION_LEFT || action == TK_ACTION_MOUSEDOWN)) {
      if (ud->entry_selected > 0)
        ud->entry_selected--;
      else if (ud->entry_count == 2) // toggle
        ud->entry_selected = 1;

      sfx_play_sample(tk->sfx, SFX_FSTYLE_LEFT_RIGHT);
    }
  }

  arrowtext_style(tk, el->userdata);
}

// Set entries to NULL and entry_count to 0 to create
// a boolean. entries will then be: ["ENABLED", "DISABLED"]
// editing_maxlen: set to 0 to disable editing completely
tk_el_t *tk_ctrl_arrowtext(tk_t *tk, tk_el_t *TK_PARENT, bg_mashup_t *bg_mashup,
                           char editing_maxlen, char **entries,
                           uint16_t entry_count, char *prefix, char *suffix,
                           char *bottom_text_high, char *bottom_text_low,
                           char *bottom_text_high_editing,
                           char *bottom_text_low_editing) {
  // fill userdata
  ud_arrowtext_t *ud = calloc(1, sizeof(ud_arrowtext_t));
  ud->editing_maxlen = editing_maxlen;
  ud->entries = entries;
  ud->entry_count = entry_count;
  ud->prefix = prefix;
  ud->suffix = suffix;
  ud->bottom_text_high = bottom_text_high;
  ud->bottom_text_low = bottom_text_low;
  ud->bottom_text_high_editing = bottom_text_high_editing;
  ud->bottom_text_low_editing = bottom_text_low_editing;

  // set defaults for booleans
  if (!entry_count && !entries) {
    ud->entries = malloc(sizeof(char *) * 2);
    ud->entries[0] = "OFF";
    ud->entries[1] = "ON";
    ud->entry_count = 2;
    ud->free_bool_default_entries = 1;
  }

  // create the layout
  TK_FLOW(
      ud->container = TK_PARENT; ud->container->bg_mashup = bg_mashup;
      ud->container->actionfunc = (void *)arrowtext_actionfunc;
      ud->container->userdata = ud; tk_el_selectable(ud->container);

      ud->left = tk_ctrl_arrow(tk, TK_PARENT, 1, (void *)arrowtext_actionfunc,
                               (void *)ud);

      if (prefix) ud->text_pre =
          tk_label(tk, TK_PARENT, prefix, GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
                   GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);

      ud->text = tk_label(tk, TK_PARENT, ud->entries[0],
                          GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
                          GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);

      if (suffix) ud->text_suf =
          tk_label(tk, TK_PARENT, suffix, GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
                   GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);

      if (editing_maxlen) {
        ud->underscore = tk_label(tk, TK_PARENT, "_",
                                  GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
        ud->underscore->actionfunc = (void *)arrowtext_actionfunc;
        ud->underscore->userdata = ud;
      }

      ud->right = tk_ctrl_arrow(tk, TK_PARENT, 0, (void *)arrowtext_actionfunc,
                                (void *)ud);

      tk_el_padding(ud->left, 5, 4, 5, 0); tk_el_padding(
          ud->right, 5 - (editing_maxlen ? ud->underscore->width : 0), 4, 5, 0);
      tk_el_padding(TK_PARENT,
                    (-1) * (ud->left->width + ud->left->padding_left +
                            ud->left->padding_right),
                    0, 0, 0););

  arrowtext_style(tk, ud);
  return ud->container;
}
