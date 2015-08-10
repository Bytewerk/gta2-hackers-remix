#include "../../../common/headers/common.h"
#include "../../chk/chk.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include "../ui_text.h"

typedef struct {
  tk_el_t *content;
  tk_el_t *poweroff;
  tk_el_t *reboot;
  ui_t *ui;
} ud_main_t;

void main_menu_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action) {
  ud_main_t *ud = (ud_main_t *)el->userdata;

  if (action == TK_ACTION_CLEANUP && ud->content->bottom_text_high) {
    free(ud->content->bottom_text_high);
  }

  if (action == TK_ACTION_DRAW_BEFORE_CONTROLS) {
    char *latest_version = chk_latest_version(ud->ui->chk);

    if (ud->content->bottom_text_high)
      free(ud->content->bottom_text_high);

    if (!latest_version || !strcmp(latest_version, G2HR_VERSION)) {
      ud->content->bottom_text_high = NULL;
    } else {
      ud->content->bottom_text_high =
          cstr_merge("UPDATE TO ", latest_version, " AVAILABLE!");
    }
  }

  if (action == TK_ACTION_ENTER) {
    char *exec_after_quit = NULL;

    if (el_selected == ud->poweroff)
      exec_after_quit =
          ini_read(ud->ui->ini_usersettings, "slotmachine", "cmd_shutdown");

    if (el_selected == ud->reboot)
      exec_after_quit =
          ini_read(ud->ui->ini_usersettings, "slotmachine", "cmd_reboot");

    if (exec_after_quit) {
      strncpy(ud->ui->net->exec_after_quit, exec_after_quit,
              G2HR_EXEC_AFTER_QUIT_LEN - 1);
      ud->ui->tk->quit = true;
    }
  }
}

tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui) {
  tk_screen_t *main_menu = tk_screen(tk, ui->credits, NULL);
  ud_main_t *ud = calloc(1, sizeof(ud_main_t));
  ud->ui = ui;

  TK_STACK_SCREEN(
      main_menu,

      tk_el_padding(TK_PARENT, 300, 250, 0, 0);

      tk_ctrl_button(tk, TK_PARENT, "CAMPAIGN",
                     bg_mashup(tk->bg, NULL, "1_play", "1", NULL), ui->play);
      tk_ctrl_button(
          tk, TK_PARENT, "SPLITSCREEN",
          bg_mashup(tk->bg, NULL, "1_options", "1", "g2hr_splitscreen"),
          ui->splitscreen);
      tk_ctrl_button(tk, TK_PARENT, "OPTIONS",
                     bg_mashup(tk->bg, NULL, "1_options", "1", NULL),
                     ui->options);

      if (ui->slotmachine) {
        tk_el_t *credits = tk_ctrl_button(
            tk, TK_PARENT, "CREDITS",
            bg_mashup(tk->bg, NULL, "1_options", "1", "g2hr_credits"),
            ui->credits);

        tk_el_padding(credits, 0, 20, 0, 0);

        ud->poweroff =
            tk_ctrl_button(tk, TK_PARENT, "POWER OFF",
                           bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), NULL);

        ud->reboot =
            tk_ctrl_button(tk, TK_PARENT, "REBOOT",
                           bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), NULL);
      } else {
        tk_ctrl_button(tk, TK_PARENT, "QUIT",
                       bg_mashup(tk->bg, NULL, "1_quit", "1", NULL),
                       ui->credits);
      }

      ud->content = TK_PARENT;
      TK_PARENT->bottom_text_low = "G2HR V" G2HR_VERSION;
      tk_event(tk, TK_PARENT, main_menu_actionfunc, ud););

  return main_menu;
}
