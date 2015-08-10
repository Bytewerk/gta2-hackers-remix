#include "../../../common/cstr/cstr.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include "../ui_text.h"

/*
        always use the ui_show_ready_screen macro (defined in ui.h)
        to show the ready screen!
*/

void ui_ready_status(ui_t *ui, char *text) {
  // free the text or set the free-text-flag
  if (ui->ready_status->flags & TK_EL_FLAG_FREE_TEXT)
    free(ui->ready_status->text);
  else
    tk_el_free_text(ui->ready_status);

  ui->ready_status->text = cstr_copy(text);
  tk_el_geocalc(ui->tk, ui->ready_status, 0, 0);
  ui->tk->redraw_needed = 1;
}

void ready_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                      tk_action_t action) {
  // Debug: go back with esc/backspace. What we actually want is
  // to go back automatically, as soon as the game is visibile.
  if (action == TK_ACTION_ESC || action == TK_ACTION_BACKSPACE) {
    tk->screen_active = ((tk_screen_t *)el)->back;
  }
}

tk_screen_t *ui_screen_ready(tk_t *tk, ui_t *ui) {
  tk_screen_t *ready = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      ready,

      // override screen action, because we have no controls
      TK_PARENT->actionfunc = ready_actionfunc;

      TK_PARENT->bg_mashup =
          bg_mashup(tk->bg, "levelcomplete", NULL, NULL, NULL);

      // GET READY text
      tk_el_t *get_ready = tk_label(tk, TK_PARENT, "GET READY...",
                                    GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
      tk_el_padding(get_ready, 236, 266, 0, 0);

      // a status line that can be set by the meta component over
      // network
      ui->ready_status =
          tk_label(tk, TK_PARENT, "", GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
      tk_el_center(ui->ready_status););

  return ready;
}
