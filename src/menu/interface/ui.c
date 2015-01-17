#include "../toolkit/toolkit.h"
#include "interface.h"

void ui_credits(tk_t *tk, ui_t *ui) { tk->screen = ui->credits; }

ui_t *ui_init(tk_t *tk) {
  ui_t *ui = malloc(sizeof(ui_t));
  tk_control_t *ctrl;

  // Credits screen (TODO: actually display credits ;) )
  ui->credits = tk_screen_create(tk, (void *)ui, NULL);
  tk_screen_setbg(tk, ui->credits, "credits", NULL, NULL);

  // Main menu
  ui->main = tk_screen_create(tk, (void *)ui, ui->credits);
  ctrl = tk_control_add(ui->main, 0x00, NULL, "Start");
  tk_control_setbg(tk, ctrl, NULL, "1_play", "1");
  ctrl = tk_control_add(ui->main, 0x00, NULL, "Options");
  tk_control_setbg(tk, ctrl, NULL, "1_options", "1");
  ctrl = tk_control_add(ui->main, 0x00, &ui_credits, "Quit");
  tk_control_setbg(tk, ctrl, NULL, "1_quit", "1");

  tk->screen = ui->main;
  return ui;
}

void ui_cleanup(tk_t *tk, ui_t *ui) {
  tk_screen_cleanup(ui->main);
  tk_screen_cleanup(ui->credits);
  tk->screen = NULL;
}
