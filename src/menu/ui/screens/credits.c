#include "../../gxt/gxt.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

// TODO: Quit at the end of the credits!
// TODO: support manually scrolling through credits
void actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                tk_action_t action) {
  if (action == TK_ACTION_BACKSPACE || action == TK_ACTION_ESC ||
      action == TK_ACTION_ENTER) {
    tk->quit = 1;
  }

  if (action == TK_ACTION_FRAMETIME) {
    el->scroll_top += 1;
    tk->redraw_needed = 1;
  }
}

tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui) {
  tk_screen_t *credits = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      credits,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "credits", NULL, NULL, NULL);
      tk_el_padding(TK_PARENT, 120, 150, 0, 120); tk_el_width(TK_PARENT, 400);
      tk_el_scrollable(TK_PARENT); TK_PARENT->actionfunc = (void *)actionfunc;

      char *original = gxt_get(tk->gxt, "credits");
      uint16_t original_length = strlen(original); uint16_t line_start = 0;

      for (int i = 0; i < original_length; i++) {
        char c = original[i];
        if (c == '#') {
          char *line = malloc(i - line_start + 1);
          strncpy(line, original + line_start, i - line_start);
          line[i - line_start] = '\0';

          tk_el_t *el = tk_label(tk, TK_PARENT, line,
                                 GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
          tk_el_center(el);
          tk_el_free_text(el);

          // skip # and color
          line_start = i + 2;
        }

      });

  return credits;
}
