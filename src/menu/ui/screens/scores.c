#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_scores(tk_t *tk, ui_t *ui) {
  tk_screen_t *scores = tk_screen(tk, NULL, NULL);
  TK_STACK_SCREEN(
      scores,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "3_tables", NULL, NULL, NULL);
      tk_el_padding(TK_PARENT, 300, 161, 0, 0);

      tk_ctrl_circle(tk, TK_PARENT, "HIGH SCORES FOR AREA",
                     bg_mashup(tk->bg, NULL, "2_level1", "2", NULL), '1', 'I',
                     '2', NULL);

      for (int i = 0; i < 5; i++) {
        TK_FLOW(tk_el_t *name = tk_label(tk, TK_PARENT, "ROBOTANARCHY");
                tk_el_width(name, 248);
                name->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_SMALL;

                tk_el_t *score = tk_label(tk, TK_PARENT, "50000");
                score->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_SMALL;)
      });
  return scores;
}
