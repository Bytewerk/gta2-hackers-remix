#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui) {
  tk_screen_t *main_menu = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(
      main_menu, tk_el_padding(TK_PARENT, 300, 250, 0, 0);
      TK_PARENT->bottom_text_low = "G2HR V" G2HR_VERSION;

      tk_ctrl_button(tk, TK_PARENT, "CAMPAIGN",
                     bg_mashup(tk->bg, NULL, "1_play", "1", NULL), ui->play);
      tk_ctrl_button(
          tk, TK_PARENT, "SPLITSCREEN",
          bg_mashup(tk->bg, NULL, "1_options", "1", "g2hr_splitscreen"),
          ui->splitscreen);
      tk_ctrl_button(tk, TK_PARENT, "OPTIONS",
                     bg_mashup(tk->bg, NULL, "1_options", "1", NULL), NULL);

      if (ui->slotmachine_enabled) {
        tk_el_t *credits = tk_ctrl_button(
            tk, TK_PARENT, "CREDITS",
            bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), ui->credits);

        tk_el_padding(credits, 0, 20, 0, 0);

        tk_ctrl_button(tk, TK_PARENT, "POWER OFF",
                       bg_mashup(tk->bg, NULL, "1_quit", "1", NULL),
                       ui->credits);
      } else {
        tk_ctrl_button(tk, TK_PARENT, "QUIT",
                       bg_mashup(tk->bg, NULL, "1_quit", "1", NULL),
                       ui->credits);
      });

  return main_menu;
}
