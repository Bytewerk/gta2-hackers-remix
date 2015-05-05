#pragma once
#include <inttypes.h>
#include <sys/types.h>

#define GTA2_MAX_PLAYERS 6

typedef struct {
  unsigned char x;
  unsigned char y;
  unsigned char w;
  unsigned char h;
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
