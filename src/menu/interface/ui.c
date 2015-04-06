#include "../toolkit/toolkit.h"
#include "interface.h"

void ui_quit(tk_t *tk, ui_t *ui) {
  exit(0); // TODO: cleanup!
}

ui_t *ui_init(tk_t *tk) {
  ui_t *ui = malloc(sizeof(ui_t));
  tk_control_t *ctrl;

  // Credits screen (TODO: actually display credits ;) )
  ui->credits = tk_screen_create(tk, (void *)ui, NULL, &ui_quit);
  tk_screen_setbg(tk, ui->credits, "credits", NULL, NULL);

  // Main menu
  ui->main = tk_screen_create(tk, (void *)ui, ui->credits, NULL);

  ctrl = tk_control_add(ui->main, TK_BUTTON, UI_TEXT_CAMPAIGNS, NULL);
  tk_control_setbg(tk, ctrl, NULL, "1_play", "1");

  ctrl = tk_control_add(ui->main, TK_BUTTON, UI_TEXT_MULTIPLAYER, NULL);
  tk_control_setbg(tk, ctrl, NULL, "1_play", "1");

  ctrl = tk_control_add(ui->main, TK_BUTTON, UI_TEXT_OPTIONS, NULL);
  tk_control_setbg(tk, ctrl, NULL, "1_options", "1");

  ctrl = tk_control_add(ui->main, TK_BUTTON, UI_TEXT_QUIT, ui->credits);
  tk_control_setbg(tk, ctrl, NULL, "1_quit", "1");

  tk->screen = ui->main;
  return ui;
}

void ui_cleanup(tk_t *tk, ui_t *ui) {
  tk_screen_cleanup(ui->main);
  tk_screen_cleanup(ui->credits);
  tk->screen = NULL;
}
