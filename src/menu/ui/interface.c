#include "interface.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

ui_t *ui_init(tk_t *tk, mmp_t *mmp, server_t *server) {
  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;
  ui->server = server;
  ui->mmp = mmp;

  // player names
  ui->player_names = malloc(sizeof(char *) * GTA2_SAVEGAME_COUNT);
  for (int i = 0; i < GTA2_SAVEGAME_COUNT; i++) {
    ui->player_names[i] = calloc(1, GTA2_PLAYERNAME_MAXLEN + 1);
    ui->player_names[i][0] = 'P';
    ui->player_names[i][1] = 'L';
    ui->player_names[i][2] = 'A';
    ui->player_names[i][3] = 'Y';
    ui->player_names[i][4] = 'E';
    ui->player_names[i][5] = 'R';
    ui->player_names[i][6] = ' ';
    ui->player_names[i][7] = '0' + i;
  }

  // screens
  tk_screen_t *levels = ui_screen_levels(tk, ui);
  tk_screen_t *credits = ui_screen_credits(tk, ui);
  tk_screen_t *scores = ui_screen_scores(tk, ui);
  tk_screen_t *play = ui_screen_play(tk, ui, scores, credits);
  tk_screen_t *splitscreen = ui_screen_splitscreen(tk, ui, levels);
  tk_screen_t *main_menu =
      ui_screen_main_menu(tk, ui, play, splitscreen, credits);

  // back links
  play->back = main_menu;
  splitscreen->back = main_menu;
  scores->back = play;
  levels->back = splitscreen;

  tk->screen_active = main_menu;
  return ui;
}

void ui_cleanup(ui_t *ui) {
  for (int i = 0; i < GTA2_SAVEGAME_COUNT; i++)
    free(ui->player_names[i]);
  free(ui->player_names);

  free(ui);
}
