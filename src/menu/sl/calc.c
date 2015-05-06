/*
        sl_calc() is a port from AutoIt3 to C99 of the function
        screen_layout_calc() of screen_layout_calc.au3, which was written
        for the alpha version of the splitscreen mod.
*/
#include "sl.h"
#include <stdio.h>
#include <stdlib.h>
#define ERR(MSG) exit(printf("ERROR in sl_calc_panel() call: %s!\n", MSG))

// writes geometrical information into the geo-variable
void sl_calc(sl_t *sl, uint16_t screen_w, uint16_t screen_h, int player_count,
             int layout_id, int player_id, sl_geo_t *scaled) {
  if (player_id > player_count || player_count >= GTA2_MAX_PLAYERS)
    ERR("Invalid player_id or players_max");

  sl_player_t *player = sl->players[player_count];
  if (player->count <= layout_id)
    ERR("Invalid layout_id");

  sl_entry_t *layout = player->layouts[layout_id];
  sl_geo_t *panel = layout->geo[player_id];

  scaled->x = screen_w * panel->x / layout->w;
  scaled->y = screen_h * panel->y / layout->h;
  scaled->w = screen_w * panel->w / layout->w;
  scaled->h = screen_h * panel->h / layout->h;
}

#undef ERR
