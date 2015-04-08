#include "../toolkit/toolkit.h"
#include "interface.h"

void ui_quit(tk_t *tk, ui_t *ui) {
  printf("ui_quit()\n");
  exit(0); // TODO: cleanup!
}

ui_t *ui_init(tk_t *tk) {
  ui_t *ui = malloc(sizeof(ui_t));

  // Credits screen (TODO: actually display credits ;) )
  ui->credits = tk_screen_create(tk, (void *)ui, NULL, &ui_quit);
  tk_screen_setbg(tk, ui->credits, "credits", NULL, NULL);

  // Main menu
  ui->main = tk_screen_create(tk, (void *)ui, ui->credits, NULL);

  tk_control_t *play;
  tk_control_add(ui->main, play, UI_TEXT_PLAY, TK_BUTTON, NULL);
  tk_control_setbg(tk, play, NULL, "1_play", "1");

  tk_control_t *options;
  tk_control_add(ui->main, options, UI_TEXT_OPTIONS, TK_BUTTON, NULL);
  tk_control_setbg(tk, options, NULL, "1_options", "1");

  tk_control_t *quit;
  tk_control_add(ui->main, quit, UI_TEXT_QUIT, TK_BUTTON,
                 data->onclick_screen = ui->credits);
  tk_control_setbg(tk, quit, NULL, "1_quit", "1");

  // Default screen is Main Menu
  tk->screen = ui->main;
  return ui;
}

void ui_cleanup(tk_t *tk, ui_t *ui) {
  tk_screen_cleanup(ui->main);
  tk_screen_cleanup(ui->credits);
  tk->screen = NULL;
}
