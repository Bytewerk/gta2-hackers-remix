#include "interface.h"
#include "../tk/controls.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

ui_t *ui_init(tk_t *tk) {
  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;

  // CREDITS
  tk_screen_t *credits = tk_screen(tk);

  TK_STACK_SCREEN(credits, TK_PARENT->bg_mashup =
                               bg_mashup(tk->bg, "credits", NULL, NULL, NULL);

                  // TODO: scroll credits automagically,
                  // make sure that the actionfunc gets called every
                  // frame. Action func must also exit the program
                  // on ESC keypress.
                  // TK_PARENT->actionfunc = credits_action ...
                  );

  // PLAY
  tk_screen_t *play = tk_screen(tk);

  TK_STACK_SCREEN(play, tk_el_padding(TK_PARENT, 300, 210, 0, 0););

  // MAIN MENU
  tk_screen_t *main_menu = tk_screen(tk);

  TK_STACK_SCREEN(
      main_menu, tk_el_padding(TK_PARENT, 300, 250, 0, 0);
      TK_PARENT->bottom_text_high = "G2HR V0.4";

      tk_ctrl_button(TK_PARENT, "PLAY",
                     bg_mashup(tk->bg, NULL, "1_play", "1", NULL), play, NULL);
      tk_ctrl_button(TK_PARENT, "OPTIONS",
                     bg_mashup(tk->bg, NULL, "1_options", "1", NULL), NULL,
                     NULL);
      tk_ctrl_button(TK_PARENT, "QUIT",
                     bg_mashup(tk->bg, NULL, "1_quit", "1", NULL), credits,
                     NULL););

  tk->screen_active = main_menu;
  return ui;
}

void ui_cleanup(ui_t *ui) { free(ui); }
