#pragma once
#include "../tk/toolkit.h"

#define GTA2_SAVEGAME_COUNT 8
#define GTA2_PLAYERNAME_MAXLEN 8

typedef struct {
  tk_t *tk;
  tk_screen_t *main;
  tk_screen_t *credits;
  tk_screen_t *play;
  tk_screen_t *options;
  char **player_names;
} ui_t;

ui_t *ui_init(tk_t *tk);
void ui_cleanup(ui_t *ui);

// screens
tk_screen_t *ui_screen_scores(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui);
tk_screen_t *ui_screen_play(tk_t *tk, ui_t *ui, tk_screen_t *scores,
                            tk_screen_t *credits);
tk_screen_t *ui_screen_main_menu(tk_t *tk, ui_t *ui, tk_screen_t *play,
                                 tk_screen_t *credits);
