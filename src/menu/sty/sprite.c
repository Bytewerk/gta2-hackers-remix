#include "sty.h"
#include <stdio.h>

/*
        Based on Black_Phoenix' tilewrite.cpp

        SDL_Texture seems to be the best choice here, according to:
                https://wiki.libsdl.org/MigrationGuide
*/

SDL_Texture *sprite_to_texture(SDL_Renderer *renderer, sty_t *sty, char silent,
                               int sprite_id) {
  // red 'E' for ERROR :)
  uint32_t error_pixels[] = {
      0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
      0xFFFF0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
      0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0x00000000, 0x00000000,
      0xFFFF0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
      0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
  };
  uint32_t *pixels;
  uint16_t vpallete = sty->pallete_index[sty->pallete_base.tile + sprite_id];
  sprite_meta_t meta = sty->sprite_index.entries[sprite_id];

  int width = meta.width;
  int height = meta.height;
  int base_x = meta.ptr % 256;
  int base_y = meta.ptr / 256;
  uint32_t base = meta.ptr - base_x - base_y * 256;
  char *blob = sty->sprite_blob.blob;

  // check if out of bounds
  char draw_error = 0;
  if (base + width * height * 256 >= sty->sprite_blob.blob_length) {
    if (!silent)
      printf("ERROR: trying to draw a sprite outside of the blob: %i\n",
             sprite_id);
    width = 5;
    height = 5;
    draw_error = 1;
  }

  // create a pixels buffer
  if (draw_error)
    pixels = error_pixels;
  else {
    pixels = malloc(sizeof(uint32_t) * width * height);

    for (int y = height - 1; y >= 0; y--) {
      for (int x = 0; x < width; x++) {
        uint32_t final_color;
        uint16_t color =
            ((uint8_t *)blob)[base + (base_x + x + (base_y + y) * 256)];

        if (!color)
          final_color = 0; // fully transparent
        else {
          uint16_t pal_id =
              (vpallete / 64) * 256 * 64 + (vpallete % 64) + color * 64;
          final_color = sty->pallete[pal_id];
          final_color |= 0xFF000000; // no alpha
        }

        pixels[x + y * width] = final_color;
      }
    }
  }

  // blend it on a new texture and return it
  SDL_Texture *tex =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, width, height);
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
  if (!draw_error)
    free(pixels);
  return tex;
}

void sty_sprite_measure(sty_t *sty, int *width, int *height, int sprite_id) {
  sprite_meta_t meta = sty->sprite_index.entries[sprite_id];
  *width = meta.width;
  *height = meta.height;
}

void sty_sprite_draw(SDL_Renderer *renderer, sty_t *sty, int sprite_id,
                     int offset_x, int offset_y, int width, int height,
                     uint32_t argb) {
  SDL_Texture *sprite = sprite_to_texture(renderer, sty, 0, sprite_id);
  SDL_Rect dest = {offset_x, offset_y, width, height};

  // set alpha and color modifications
  SDL_SetTextureAlphaMod(sprite, (uint8_t)(argb >> 24));
  SDL_SetTextureColorMod(sprite, (uint8_t)(argb >> 16), (uint8_t)(argb >> 8),
                         (uint8_t)(argb));

  SDL_RenderCopy(renderer, sprite, NULL, &dest);
  SDL_DestroyTexture(sprite);
}
