#include "interface.h"
#include "../tk/controls.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

void ui_credits_action(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action) {
  if (action == TK_ACTION_BACK || action == TK_ACTION_ENTER) {
    tk->quit = 1;
  }
  // TODO: scroll credits on every frame (define frame action)
  // also maybe add controls to pause and manually scroll?
}

ui_t *ui_init(tk_t *tk) {
  // TODO: add all important controls to the ui struct, so we can
  // change their properties outside of this function

  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;

  // CREDITS
  tk_screen_t *credits = tk_screen(tk, NULL, (void *)ui_credits_action);

  TK_STACK_SCREEN(credits, TK_PARENT->bg_mashup =
                               bg_mashup(tk->bg, "credits", NULL, NULL, NULL););

  // PLAY
  tk_screen_t *play = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      play, tk_el_padding(TK_PARENT, 300, 210, 0, 0);

      // FIXME: this isn't a button, implement text fields!
      tk_ctrl_button(tk, TK_PARENT, "PLAYER 0",
                     bg_mashup(tk->bg, NULL, "2_name", "2", NULL), NULL, NULL);

      tk_el_t *resume = tk_ctrl_button(tk, TK_PARENT, "RESUME SAVED STATUS",
                                       NULL, NULL, NULL);
      tk_el_disabled(resume);

      tk_ctrl_button(tk, TK_PARENT, "VIEW HIGH SCORES",
                     bg_mashup(tk->bg, NULL, "2_league", "2", NULL), NULL,
                     NULL);

      tk_ctrl_circle(tk, TK_PARENT, "START PLAY IN AREA",
                     bg_mashup(tk->bg, NULL, "2_level1", "2", NULL), '1', '3',
                     '2', NULL);

      );

  // MAIN MENU
  tk_screen_t *main_menu = tk_screen(tk, credits, NULL);

  TK_STACK_SCREEN(
      main_menu, tk_el_padding(TK_PARENT, 300, 250, 0, 0);
      TK_PARENT->bottom_text_low = "G2HR V0.4";

      tk_ctrl_button(tk, TK_PARENT, "PLAY",
                     bg_mashup(tk->bg, NULL, "1_play", "1", NULL), play, NULL);
      tk_ctrl_button(tk, TK_PARENT, "OPTIONS",
                     bg_mashup(tk->bg, NULL, "1_options", "1", NULL), NULL,
                     NULL);
      tk_ctrl_button(tk, TK_PARENT, "QUIT",
                     bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), credits,
                     NULL););

  // now that we have the main menu, set the back links
  play->back = main_menu;

  tk->screen_active = main_menu;
  return ui;
}

void ui_cleanup(ui_t *ui) { free(ui); }
