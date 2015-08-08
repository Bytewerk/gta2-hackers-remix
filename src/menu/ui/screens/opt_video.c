#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include <stdbool.h>

// USERDATA STRUCT
typedef struct {
  ui_t *ui;
  tk_el_t *fullscreen;

} ud_opt_video_t;

void opt_video_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key) {
  ud_opt_video_t *ud = (ud_opt_video_t *)el->userdata;

  if (action == TK_ACTION_LEFT || action == TK_ACTION_RIGHT ||
      action == TK_ACTION_ENTER) {
    bool is_fullscreen =
        ((ud_arrowtext_t *)ud->fullscreen->userdata)->entry_selected;

    ini_modify(ud->ui->ini_settings, "video", "fullscreen",
               is_fullscreen ? "true" : "false", true);
    ui_apply_video_config(ud->ui);

    ini_save(ud->ui->ini_settings, NULL, false, false);
  }
}

tk_screen_t *ui_screen_opt_video(tk_t *tk, ui_t *ui) {
  ud_opt_video_t *ud = calloc(1, sizeof(ud_opt_video_t));
  ud->ui = ui;

  tk_screen_t *ret = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(
      ret,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, NULL, "1_options", "1", NULL);
      TK_PARENT->bottom_text_low = "SEE ALSO: GIT.IO/G2HR_VIDEO";

      TK_STACK(ret->el_content_container = TK_PARENT; TK_PARENT->userdata = ud;
               TK_PARENT->actionfunc = (void *)opt_video_actionfunc;

               tk_el_padding(TK_PARENT, 300, 150, 0, 0);

               ud->fullscreen =
                   tk_ctrl_boolean(tk, TK_PARENT, NULL, "FULLSCREEN: ");

               ((ud_arrowtext_t *)ud->fullscreen->userdata)->entry_selected =
                   (strcmp(ini_read(ui->ini_settings, "video", "fullscreen"),
                           "true") == 0);));

  return ret;
}
