#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui, tk_screen_t *scores,
                            tk_screen_t *credits) {
  tk_screen_t *play = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      play, tk_el_padding(TK_PARENT, 300, 210, 0, 0);

      // FIXME: this isn't a button, implement text fields!

      tk_ctrl_arrowtext(
          tk, TK_PARENT, bg_mashup(tk->bg, NULL, "2_name", "2", NULL),
          ui->player_names, GTA2_SAVEGAME_COUNT, 0, GTA2_PLAYERNAME_MAXLEN,
          NULL, 0, "ENTER:  EDIT NAME", "DELETE:  DELETE PLAYER",
          "ENTER:  ENTER NAME", "BACKSPACE:  DELETE LETTER");

      tk_el_t *resume = tk_ctrl_button(tk, TK_PARENT, "RESUME SAVED STATUS",
                                       NULL, NULL, NULL);
      tk_el_disabled(resume);

      tk_ctrl_button(tk, TK_PARENT, "VIEW HIGH SCORES",
                     bg_mashup(tk->bg, NULL, "2_league", "2", NULL), scores,
                     NULL);

      tk_ctrl_circle(tk, TK_PARENT, "START PLAY IN AREA",
                     bg_mashup(tk->bg, NULL, "2_level1", "2", NULL), '1', '3',
                     0, 0, '2', NULL);

      );

  return play;
}
