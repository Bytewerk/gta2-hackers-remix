#pragma once
#include <sys/types.h>

typedef struct cfg_t {
  struct cfg_t *next;
  char *key;
  char *value;
} cfg_t;

typedef struct {
  char **values;
  int count;
} cfg_split_t;

cfg_t *cfg_load(char *filename);
char *cfg_read(cfg_t *cfg, char *key);
void cfg_cleanup(cfg_t *cfg);

cfg_split_t *cfg_split_value(cfg_t *cfg, char *key, char delimeter);
void cfg_split_cleanup(cfg_split_t *split);
