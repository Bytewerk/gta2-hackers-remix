#include "../../server/menu_meta_protocol.h"
#include "../../server/server.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

typedef struct {
  ui_t *ui;
  tk_el_t *start;

} ud_play_t;

void play_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                     tk_action_t action) {
  ud_play_t *ud = (ud_play_t *)el->userdata;
  if (el_selected == ud->start && action == TK_ACTION_ENTER) {
    printf("starting the game here!\n");
    server_send(ud->ui->server, G2HR_MENU_META_SINGLEPLAYER, 0);
  }
}

tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui, tk_screen_t *scores,
                            tk_screen_t *credits) {
  tk_screen_t *play = tk_screen(tk, NULL, NULL);

  ud_play_t *ud = malloc(sizeof(ud_play_t));
  ud->ui = ui;

  TK_STACK_SCREEN(
      play, tk_el_padding(TK_PARENT, 300, 210, 0, 0);

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

      ud->start = tk_ctrl_circle(tk, TK_PARENT, "START PLAY IN AREA",
                                 bg_mashup(tk->bg, NULL, "2_level1", "2", NULL),
                                 '1', '3', 0, 0, '2', NULL);

      // FIXME: we can't attach an actionfunc to the screen,
      // because screens already have one. maybe we can make this
      // more elegant (just pack it in a macro?)
      TK_STACK(TK_PARENT->actionfunc = play_actionfunc;
               TK_PARENT->userdata = ud;));

  return play;
}
