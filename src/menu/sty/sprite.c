#include "sty.h"
#include <stdio.h>

// Based on Black_Phoenix tilewrite.cpp

SDL_Texture *sty_sprite(SDL_Renderer *renderer, sty_t *sty, int sprite_id) {
  uint16_t vpallete = sty->pallete_index[sty->pallete_base.tile + sprite_id];
  sprite_meta_t meta = sty->sprite_index.entries[sprite_id];

  int width = meta.width;
  int height = meta.height;
  int base_x = meta.ptr % 256;
  int base_y = meta.ptr / 256;
  int base = meta.ptr - base_x - base_y * 256;

  char *blob = sty->sprite_blob.blob;

  uint32_t *pixels = malloc(sizeof(uint32_t) * width * height);
  SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STATIC, width, height);

  // fill the pixels buffer
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      uint16_t color =
          ((uint16_t *)blob)[base + (base_x + x + (base_y + y) * 256)];
      uint16_t pal_id =
          (vpallete / 64) * 256 * 64 + (vpallete % 64) + color * 64;
      uint16_t final_color = (sty->pallete[pal_id]) & 0xFFFFFF;

      pixels[x + y * width] = final_color;
    }
  }

  SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
  return tex;
}
