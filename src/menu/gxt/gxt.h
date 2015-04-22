#pragma once
#include <stdint.h>

typedef struct gxt_entry_t {
  struct gxt_entry_t *next;
  char *id;
  char *text;
} gxt_entry_t;

typedef struct {
  gxt_entry_t *entries;
  char lang; // 'E' for english etc.
} gxt_t;

gxt_t *gxt_load(const char *filename);
char *gxt_get(gxt_t *gxt, char *id);
void gxt_cleanup(gxt_t *gxt);
