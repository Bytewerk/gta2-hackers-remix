#include "cfg.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test cfg_file\n");
  cfg_t *cfg = cfg_load((char *)argv[1]);

  cfg_t *current = cfg;
  while (current) {
    printf("%s: %s\n", current->key, current->value);
    current = current->next;
  }

  printf("TIME VALUES:\n");
  cfg_split_t *time = cfg_split_value(cfg, "multiplayer/time", ' ');
  for (int i = 0; i < time->count; i++)
    printf("    %i/%i: '%s'\n", i + 1, time->count, time->values[i]);
  cfg_split_cleanup(time);

  cfg_cleanup(cfg);
}
