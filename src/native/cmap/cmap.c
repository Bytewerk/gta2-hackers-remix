#include "cmap.h"
#include "../../common/cfg/cfg.h"
#include "../../common/common.h"
#include <stdio.h>
#include <string.h>

/*
        TODO: add move file loading from mmp to cfg and also use it here
        to load all ini files in that directory!

*/

void cmap_map_action(cmap_t *cmap, cfg_t *cfg, char *str_action,
                     cmap_action_t cmap_action) {
  // transform the str_action in the form of "WALKING_ENTER_CAR"
  // into the form we actually need to find the config value:
  // walking/enter-car
  int len = strlen(str_action);
  char cfg_key[strlen(str_action) + 1];
  char seen_underscore = 0;
  for (int i = 0; i < len; i++) {
    char c = str_action[i];
    if (c == '_') {
      if (!seen_underscore)
        c = '/';
      else {
        c = '-';
        seen_underscore = 1;
      }
    } else
      c = c - 'A' + 'a';

    cfg_key[i] = c;
  }
  cfg_key[len] = '\0';

  printf("converted key for lookup from %s to %s.\n", str_action, cfg_key);

  cfg_split_t *split = cfg_split_value(cfg, cfg_key, ',');
  for (int i = 0; i < split->count; i++) {
    // TODO: check if this can be translated to a valid sdl con-
    // troller key, and add it to the cmap struct or print an error.
    printf("#%i: %s\n", i, split->values[i]);
  }
  cfg_split_cleanup(split);
}

#define str(x) #x
#define MAPPING(ACTION)                                                        \
  cmap_map_action(cmap, cfg, str(ACTION), G2HR_CMAP_##ACTION)

cmap_t *cmap_init() {
  cmap_t *cmap = calloc(1, sizeof(cmap_t));

  cfg_t *cfg = cfg_load("data/controller-mappings/default.ini", 0);

  MAPPING(WALKING_FORWARD);

  cfg_cleanup(cfg);
  return cmap;
}

#undef MAPPING
#undef str

void cmap_cleanup(cmap_t *cmap) { free(cmap); }
