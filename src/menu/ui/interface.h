#pragma once
#include "../../common/cfg/cfg.h"
#include "../mmp/mmp.h"
#include "../server/server.h"
#include "../tk/toolkit.h"

#define GTA2_SAVEGAME_COUNT 8
#define GTA2_PLAYERNAME_MAXLEN 8

typedef struct {
  tk_t *tk;
  tk_screen_t *main;
  tk_screen_t *credits;
  tk_screen_t *play;
  tk_screen_t *options;
  tk_screen_t *splitscreen;
  tk_screen_t *scores;
  tk_screen_t *levels;

  server_t *server;
  mmp_t *mmp;
  char **player_names;

  cfg_t *g2hr_config;
  cfg_split_t *multiplayer_time_values;
  char slotmachine_enabled;
} ui_t;

ui_t *ui_init(tk_t *tk, mmp_t *mmp, server_t *server);
void ui_cleanup(ui_t *ui);

// screens
tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_scores(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui);
