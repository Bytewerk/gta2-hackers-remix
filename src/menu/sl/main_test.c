#include "sl.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test path/to/screen_layouts.cfg\n");

  sl_t *sl = sl_init((char *)argv[1]);

  uint16_t screen_w = 1920;
  uint16_t screen_h = 1080;

  printf("Example screen resolution: %ix%i\n", screen_w, screen_h);

  for (int i = 0; i < GTA2_MAX_PLAYERS; i++) {
    sl_player_t *player = sl->players[i];

    for (int j = 0; j < player->count; j++) {
      uint16_t layout_w = player->layouts[j]->w;
      uint16_t layout_h = player->layouts[j]->h;

      printf("========= Layout %i-%c   (w: %4i, h: %4i) ==========\n", i + 1,
             j + 'A', layout_w, layout_h);

      for (int k = 0; k <= i; k++) {
        sl_geo_t scaled;
        sl_calc(sl, screen_w, screen_h, i, j, k, &scaled);
        printf("   P%i x: %4i, y: %4i, w: %4i, h: %4i\n", k + 1, scaled.x,
               scaled.y, scaled.w, scaled.h);
      }
    }
  }

  sl_cleanup(sl);

  printf("cleaned up!\n");
}
