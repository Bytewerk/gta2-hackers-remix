#include "interface.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

ui_t *ui_init(tk_t *tk, mmp_t *mmp, server_t *server) {
  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;
  ui->server = server;
  ui->mmp = mmp;
  ui->g2hr_config = cfg_load("data/g2hr.cfg", 0);
  ui->multiplayer_time_values =
      cfg_split_value(ui->g2hr_config, "multiplayer/time", ' ');
  ui->slotmachine_enabled =
      !strcmp(cfg_read(ui->g2hr_config, "slotmachine/enabled"), "true");

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
  ui->levels = ui_screen_levels(tk, ui);
  ui->credits = ui_screen_credits(tk, ui);
  ui->scores = ui_screen_scores(tk, ui);
  ui->play = ui_screen_play(tk, ui);
  ui->splitscreen = ui_screen_splitscreen(tk, ui);
  ui->main = ui_screen_main_menu(tk, ui);

  // back links
  ui->play->back = ui->main;
  ui->splitscreen->back = ui->main;
  ui->scores->back = ui->play;
  ui->levels->back = ui->splitscreen;

  tk->screen_active = ui->main;
  return ui;
}

void ui_cleanup(ui_t *ui) {
  for (int i = 0; i < GTA2_SAVEGAME_COUNT; i++)
    free(ui->player_names[i]);
  free(ui->player_names);

  cfg_split_cleanup(ui->multiplayer_time_values);
  cfg_cleanup(ui->g2hr_config);

  free(ui);
}
