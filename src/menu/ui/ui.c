#include "ui.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

ui_t *ui_init(tk_t *tk, mmp_t *mmp, server_t *server, sl_t *sl,
              cfg_t *g2hr_config) {
  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;
  ui->server = server;
  ui->mmp = mmp;
  ui->sl = sl;
  ui->g2hr_config = g2hr_config;
  ui->multiplayer_time_values =
      cfg_split_value(g2hr_config, "multiplayer/time", ' ');
  ui->slotmachine_enabled =
      !strcmp(cfg_read(g2hr_config, "slotmachine/enabled"), "true");

  // letters
  ui->letters = malloc(sizeof(char *) * G2HR_UI_LETTERS_COUNT);
  for (int i = 0; i < G2HR_UI_LETTERS_COUNT; i++) {
    ui->letters[i] = malloc(2);
    ui->letters[i][0] = 'A' + i;
    ui->letters[i][1] = '\0';
  }

  // numbers
  ui->numbers = malloc(sizeof(char *) * G2HR_UI_NUMBERS_COUNT);
  for (int i = 0; i < G2HR_UI_NUMBERS_COUNT; i++) {
    ui->numbers[i] = malloc(2);
    ui->numbers[i][0] = '1' + i;
    ui->numbers[i][1] = '\0';
  }

  // game types
  ui->game_types = malloc(sizeof(char *) * G2HR_UI_GAME_TYPES_COUNT);
  ui->game_types[0] = "FRAGS";
  ui->game_types[1] = "POINTS";
  ui->game_types[2] = "TAG";

  // maps
  ui->maps = malloc(sizeof(char *) * mmp->file_count);
  for (size_t i = 0; i < mmp->file_count; i++)
    ui->maps[i] = cfg_read(mmp->files[i]->data, "MapFiles/Description");

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

  for (int i = 0; i < G2HR_UI_LETTERS_COUNT; i++)
    free(ui->letters[i]);
  free(ui->letters);

  for (int i = 0; i < G2HR_UI_NUMBERS_COUNT; i++)
    free(ui->numbers[i]);
  free(ui->numbers);

  free(ui->maps);
  free(ui->game_types);

  cfg_split_cleanup(ui->multiplayer_time_values);
  free(ui);
}
