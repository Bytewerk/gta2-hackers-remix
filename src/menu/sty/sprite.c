#include "sty.h"
#include <stdio.h>

/*
        Based on Black_Phoenix' tilewrite.cpp

        tracing in tilewrite.cpp:
                save_sprite(name, sprite_id) // creates a bitmap file with the
                        sprite
                - gets called in sty2tex.cpp
                - source in tilewrite.cpp
                - writes a BMP header, calls write_sprite_bmp(sprite_id);
                        from the same file!

        write_sprite_bmp(sprite_id)


        - added debug print code to both my and Black_Phoenix code
        - the final_color is completely different!
*/

SDL_Texture *sty_sprite(SDL_Renderer *renderer, sty_t *sty, int sprite_id) {
  uint16_t vpallete = sty->pallete_index[sty->pallete_base.tile + sprite_id];
  sprite_meta_t meta = sty->sprite_index.entries[sprite_id];

  int width = meta.width;
  int height = meta.height;
  int base_x = meta.ptr % 256;
  int base_y = meta.ptr / 256;
  uint32_t base = meta.ptr - base_x - base_y * 256;
  char *blob = sty->sprite_blob.blob;

  printf("Debug info:\n");
  printf("\tsprite_id: %i\n", sprite_id);
  printf("\twidth:     %i\n", width);
  printf("\theight:    %i\n", height);
  printf("\tptr:       %i\n", meta.ptr);
  printf("\tbase_x:    %i\n", base_x);
  printf("\tbase_y:    %i\n", base_y);
  printf("\tbase:      %i\n", base); /* base is always 0 - wut? */

  uint32_t *pixels = malloc(sizeof(uint32_t) * width * height);
  SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STATIC, width, height);

  // fill the pixels buffer
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      uint16_t color =
          ((uint8_t *)blob)[base + (base_x + x + (base_y + y) * 256)];
      uint16_t pal_id =
          (vpallete / 64) * 256 * 64 + (vpallete % 64) + color * 64;
      uint16_t final_color = (sty->pallete[pal_id]) & 0xFFFFFF;

      // TODO: alpha color!

      pixels[x + y * width] = final_color;
      printf("%8i ", color);
    }
    printf("\n");
  }
  printf("\n");

  SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
  free(pixels);
  return tex;
}
