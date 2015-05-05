#include "sl.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test path/to/screen_layouts.cfg\n");

  sl_t *sl = sl_init((char *)argv[1]);

  for (int i = 0; i < GTA2_MAX_PLAYERS; i++) {
    sl_player_t *player = sl->players[i];
    printf("%i Players: %i Layouts\n", i + 1, player->count);
  }

  sl_cleanup(sl);

  printf("cleaned up!\n");
}
