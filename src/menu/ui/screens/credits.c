#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

void actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                tk_action_t action) {
  if (action == TK_ACTION_BACKSPACE || action == TK_ACTION_ESC ||
      action == TK_ACTION_ENTER) {
    tk->quit = 1;
  }

  if (action == TK_ACTION_FRAMETIME) {
    // scroll down here!
  }
}

tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui) {
  tk_screen_t *credits = tk_screen(tk, NULL, (void *)actionfunc);

  TK_STACK_SCREEN(
      credits,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "credits", NULL, NULL, NULL);
      tk_el_padding(TK_PARENT, 130, 150, 0, 130);

      tk_label(tk, TK_PARENT, "Here should be the credits.")->font_id =
          GTA2_FONT_FSTYLE_GRADIENT_BLACK_NORMAL;);

  return credits;
}
