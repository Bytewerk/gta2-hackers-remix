#pragma once
#include <inttypes.h>
#include <sys/types.h>

#define GTA2_MAX_PLAYERS 6

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
} sl_geo_t;

typedef struct sl_entry_t {
  struct sl_entry_t *next;
  sl_geo_t **geo;
  unsigned char w;
  unsigned char h;
} sl_entry_t;

typedef struct {
  sl_entry_t **layouts;
  uint16_t count;
} sl_player_t;

typedef struct { sl_player_t *players[GTA2_MAX_PLAYERS]; } sl_t;

sl_t *sl_init(char *filename);
void sl_cleanup(sl_t *sl);

void sl_calc(sl_t *sl, uint16_t screen_w, uint16_t screen_h, int player_count,
             int layout_id, int player_id, sl_geo_t *geo);
