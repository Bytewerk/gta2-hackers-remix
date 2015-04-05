#include "sty.h"
#include <stdio.h>

// Based on Black_Phoenix tilewrite.cpp

// DEBUG: print a sprite to the terminal.
// Later this should create a SDL_Surface for caching the whole font.
void sty_sprite(sty_t *sty, int sprite_id) {
  uint16_t vpallete = sty->pallete_index[sty->pallete_base.tile + sprite_id];
  sprite_meta_t meta = sty->sprite_index.entries[sprite_id];

  int width = meta.width;
  int height = meta.height;
  int base_x = meta.ptr % 256;
  int base_y = meta.ptr / 256;
  int base = meta.ptr - base_x - base_y * 256;

  char *blob = sty->sprite_blob.blob;

  printf("SPRITE %i; WIDTH: %i; HEIGHT: %i; base_x: %i; base_y: %i\n",
         sprite_id, width, height, base_x, base_y);

  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      uint16_t color =
          ((uint16_t *)blob)[base + (base_x + x + (base_y + y) * 256)];
      uint16_t pal_id =
          (vpallete / 64) * 256 * 64 + (vpallete % 64) + color * 64;
      uint16_t final_color = (sty->pallete[pal_id]) & 0xFFFFFF;

      if (final_color)
        printf(".", final_color);
      else
        printf(" ");
    }
    printf("\n");
  }
  printf("\n");
}
