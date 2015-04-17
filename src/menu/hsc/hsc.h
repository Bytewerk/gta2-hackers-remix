#pragma once
#include <stdint.h>

// right value is 12, but it currently segfaults there
#define GTA2_LEVEL_COUNT 10

typedef struct {
  char name[9];
  uint32_t score;
} hsc_entry_t;

typedef struct { hsc_entry_t entry[5]; } hsc_level_t;

typedef struct { hsc_level_t level[10]; } hsc_t;

hsc_t *hsc_load(hsc_t *hsc, const char *filename);
void hsc_cleanup(hsc_t *hsc);
