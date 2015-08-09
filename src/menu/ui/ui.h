#pragma once
#include "../../common/cstr/cstr.h"
#include "../../common/ini/ini.h"
#include "../chk/chk.h"
#include "../mmp/mmp.h"
#include "../net/menu_net.h"
#include "../sl/sl.h"
#include "../tk/toolkit.h"
#include <stdbool.h>

#define GTA2_SAVEGAME_COUNT 8
#define GTA2_PLAYERNAME_MAXLEN 8
#define G2HR_UI_LETTERS_COUNT ('Z' - 'A' + 1)
#define G2HR_UI_NUMBERS_COUNT 10
#define G2HR_UI_GAME_TYPES_COUNT 3

// maybe add this as option to g2hr.ini?
#define G2HR_MUSIC_FADE_OUT_TIME_MS 500

typedef struct {
  tk_t *tk;
  tk_screen_t *main;
  tk_screen_t *credits;
  tk_screen_t *play;
  tk_screen_t *options;
  tk_screen_t *opt_audio;
  tk_screen_t *opt_video;
  tk_screen_t *opt_controls;
  tk_screen_t *opt_network;
  tk_screen_t *splitscreen;
  tk_screen_t *scores;
  tk_screen_t *levels;
  tk_screen_t *ready;
  tk_screen_t *first_run;

  chk_t *chk;
  net_t *net;
  mmp_t *mmp;
  sl_t *sl;

  // configs - the settings file gets written automatically, the user-
  // settings file does only get modified by the user.
  ini_t *ini_usersettings;
  ini_t *ini_settings;

  // values for arrowtext controls
  cstr_split_t *multiplayer_time_values;
  cstr_split_t *menu_upscaling_values;
  cstr_split_t *ingame_lighting_values;
  cstr_split_t *gamma_values;

  // these are used with the arrowtext control and a prefix
  char **letters;
  char **numbers;
  char **game_types;
  char **maps; // count: ui->mmp->file_count

  // synced value between the screens levelselect and splitscreen.
  // value is an index of the ui->maps array (with the initial sorting
  // by map name, alphabetically - later we might be able to change
  // the sorting inside the ui->mmp struct!)
  size_t map_selected;

  // used for the play screen
  char **player_names;

  // is the running game a singleplayer game? gets set at the start
  // of a new game
  bool singleplayer;

  // synced from the native component
  uint16_t controllers_connected;
} ui_t;

ui_t *ui_init(tk_t *tk, mmp_t *mmp, net_t *net, sl_t *sl);

// configs related
void ui_init_configs(ui_t *ui);
void ui_cleanup_configs(ui_t *ui);
void ui_apply_video_config(ui_t *ui);
void ui_apply_audio_config(ui_t *ui);

void ui_callback_for_meta(char *msg, void *userdata);
void ui_callback_for_native(TCPsocket sock, char header, void *userdata);
void ui_cleanup(ui_t *ui);

// screens
tk_screen_t *ui_screen_first_run(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_scores(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_ready(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_options(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_opt_audio(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_opt_video(tk_t *tk, ui_t *ui);

// screen functions required else where
void splitscreen_set_players(ui_t *ui);

#define ui_show_ready_screen(UI, PREVIOUS_SCREEN_IN_UI_STRUCT)                 \
  {                                                                            \
    UI->ready->back = UI->PREVIOUS_SCREEN_IN_UI_STRUCT;                        \
    UI->tk->screen_active = UI->ready;                                         \
  }
