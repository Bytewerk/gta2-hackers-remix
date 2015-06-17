#include "../../tk/ctrl/controls.h"
#include "../ui.h"
#include "../ui_text.h"

typedef struct {
  ui_t *ui;
  tk_el_t *start;
  tk_el_t *bonus;

} ud_play_t;

void play_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                     tk_action_t action) {
  ud_play_t *ud = (ud_play_t *)el->userdata;
  ud->ui->singleplayer = true;

  if (action == TK_ACTION_ENTER &&
      (el_selected == ud->start || el_selected == ud->bonus)) {
    ui_show_ready_screen(ud->ui, play);

    char *buffer = malloc(100);
    snprintf(buffer, 100, "SINGLEPLAYER GTA2 %s %c",
             (el_selected == ud->start) ? "NORMAL" : "BONUS",
             ((ud_circle_t *)el_selected->userdata)->value_str[0]);
    net_send_to_meta(ud->ui->net, buffer, 1);
  }
}

tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui) {
  tk_screen_t *play = tk_screen(tk, NULL, NULL);

  ud_play_t *ud = malloc(sizeof(ud_play_t));
  ud->ui = ui;

  TK_STACK_SCREEN(
      play,

      tk_ctrl_arrowtext(
          tk, TK_PARENT, bg_mashup(tk->bg, NULL, "2_name", "2", NULL),
          GTA2_PLAYERNAME_MAXLEN, ui->player_names, GTA2_SAVEGAME_COUNT, NULL,
          NULL, "ENTER:  EDIT NAME", "DELETE:  DELETE PLAYER",
          "ENTER:  ENTER NAME", "BACKSPACE:  DELETE LETTER");

      tk_el_t *resume =
          tk_ctrl_button(tk, TK_PARENT, "RESUME SAVED STATUS", NULL, NULL);
      tk_el_disabled(resume);

      tk_ctrl_button(tk, TK_PARENT, "VIEW HIGH SCORES",
                     bg_mashup(tk->bg, NULL, "2_league", "2", NULL),
                     ui->scores);

      ud->start = tk_ctrl_circle(tk, TK_PARENT, "START PLAY IN AREA",
                                 bg_mashup(tk->bg, NULL, "2_level1", "2", NULL),
                                 '1', '3', 0, 0, '1');

      ud->bonus = tk_ctrl_circle(tk, TK_PARENT, "BONUS STAGE",
                                 bg_mashup(tk->bg, NULL, "2_bonus1", "2", NULL),
                                 'A', 'I', 0, 0, 'A');

      tk_el_padding(TK_PARENT, 300, 210, 0, 0);
      tk_event(tk, TK_PARENT, play_actionfunc, ud););

  return play;
}
