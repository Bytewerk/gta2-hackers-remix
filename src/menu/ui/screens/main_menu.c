#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui, tk_screen_t *play,
                                 tk_screen_t *splitscreen,
                                 tk_screen_t *credits) {
  tk_screen_t *main_menu = tk_screen(tk, credits, NULL);

  TK_STACK_SCREEN(
      main_menu, tk_el_padding(TK_PARENT, 300, 250, 0, 0);
      TK_PARENT->bottom_text_low = "G2HR V0.4";

      tk_ctrl_button(tk, TK_PARENT, "CAMPAIGN",
                     bg_mashup(tk->bg, NULL, "1_play", "1", NULL), play, NULL);
      tk_ctrl_button(
          tk, TK_PARENT, "SPLITSCREEN",
          bg_mashup(tk->bg, NULL, "1_options", "1", "g2hr_splitscreen"),
          splitscreen, NULL);
      tk_ctrl_button(tk, TK_PARENT, "OPTIONS",
                     bg_mashup(tk->bg, NULL, "1_options", "1", NULL), NULL,
                     NULL);
      tk_ctrl_button(tk, TK_PARENT, "QUIT",
                     bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), credits,
                     NULL););

  return main_menu;
}
