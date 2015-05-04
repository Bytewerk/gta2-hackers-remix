#pragma once
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
  sl_geo_t **geometry;
  unsigned char w;
  unsigned char h;
} sl_entry_t;

typedef struct { sl_entry_t **players; } sl_t;

sl_t *sl_init(char *filename);
void sl_cleanup(sl_t *sl);
