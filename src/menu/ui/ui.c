#include "ui.h"
#include "../tk/toolkit.h"
#include "ui_text.h"

void ui_start_meta(ui_t *ui) {
  if (strcmp(SDL_GetPlatform(), "Windows")) {
    printf("NOTE: You have not compiled the menu for win32, so the"
           " 'meta' component will not be started. The commands that"
           " *would* be sent to it are shown in the terminal anyway."
           " Enjoy debugging :)\n");
    return;
  }

  // transform the port into a string
  char port_str[10];
  snprintf(port_str, 9, "%i", ui->net->server_port);

  // set up the full launch command
  char *cmd;
  ini_t *ini = ui->ini_usersettings;
  bool debug_meta = !strcmp(ini_read(ini, "debug-meta", "enabled"), "true");
  if (!debug_meta) {
    // launch the binary
    cmd = cstr_merge("start bin/meta.exe ", port_str);
  } else {
    // launch the interpreter
    cmd = cstr_merge("start /min cmd /c \"",
                     ini_read(ini, "debug-meta", "autoit3_path"),
                     "AutoIt3.exe\" /ErrorStdOut \"",
                     ini_read(ini, "debug-meta", "meta_path"), "meta.au3\" ",
                     port_str, " ^> meta_errors.log");
  }

  printf("starting the meta component:\n");
  printf("> %s\n", cmd);
  system(cmd);
  free(cmd);

  if (debug_meta)
    SDL_Delay(1000);
}

ui_t *ui_init(tk_t *tk, mmp_t *mmp, net_t *net, sl_t *sl) {
  ui_t *ui = calloc(1, sizeof(ui_t));

  // put all arguments in the struct
  ui->tk = tk;
  ui->net = net;
  ui->net->meta_recv_callback = (void *)ui_callback_for_meta;
  ui->net->native_recv_callback = (void *)ui_callback_for_native;
  ui->net->userdata = (void *)ui;
  ui->mmp = mmp;
  ui->sl = sl;

  ui_init_configs(ui);
  ui_start_meta(ui);

  // do update check in background, if the user has enabled it
  if (!strcmp(ini_read(ui->ini_settings, "ui", "update_check_enabled"),
              "true")) {
    ui->chk = chk_init(tk->pref_path, true);
  }

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
    ui->maps[i] = ini_read(mmp->files[i]->data, "MapFiles", "Description");

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
  ui->ready = ui_screen_ready(tk, ui);
  ui->levels = ui_screen_levels(tk, ui);
  ui->credits = ui_screen_credits(tk, ui);
  ui->scores = ui_screen_scores(tk, ui);
  ui->play = ui_screen_play(tk, ui);
  ui->splitscreen = ui_screen_splitscreen(tk, ui);
  ui->options = ui_screen_options(tk, ui);
  ui->main = ui_screen_main_menu(tk, ui);

  // back links
  ui->play->back = ui->main;
  ui->splitscreen->back = ui->main;
  ui->options->back = ui->main;
  ui->scores->back = ui->play;
  ui->levels->back = ui->splitscreen;

  // ask if updates should be enabled on first run
  if (strcmp(ini_read(ui->ini_settings, "ui", "update_check_enabled"), "ask"))
    tk->screen_active = ui->main;
  else
    tk->screen_active = ui_screen_first_run(tk, ui);

  SDL_ShowWindow(ui->tk->window);
  SDL_RaiseWindow(ui->tk->window);
  sfx_play_song(ui->tk->sfx, SFX_SONG_MAINMENU);
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

  ui_cleanup_configs(ui);
  chk_cleanup(ui->chk);

  free(ui);
}
