#include "menu.h"
#include <stdint.h>

// SDL2 pixel drawing:
// http://www.programmersranch.com/2014/02/sdl2-pixel-drawing.html

// TODO: create an enum for font IDs
// TODO: check lengths!
void letter_draw(menu_t *menu, char font_id, uint16_t left, uint16_t top,
                 char letter) {
  // get the sprite information
  int sprite_id = (letter - 'a') + menu->fstyle->font_base.base[font_id];
  sprite_entry_t sprite_entry =
      menu->fstyle->sprite_index.sprite_entries[sprite_id];
  int width = sprite_entry.width;
  int height = sprite_entry.height;
  printf("sprite_id: %i, w: %i, h: %i\n", sprite_id, width, height);

  // max. allowed size: 128x64
  if (width > 128 || height > 64)
    exit(printf("Sprite %i has invalid width and/or height.\n", sprite_id));

  // save the sprite in a pixel array
  char *sprite = menu->fstyle->sprite_blob.blob + sprite_entry.ptr;

  // output the sprite - debug!
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++)
      printf("%02x ", sprite[(i * width) + j] & 0xff);
    printf("\n");
  }

  uint32_t *pixels = malloc(sizeof(uint32_t) * width * height);

  // create a new texture
  SDL_Texture *tex = SDL_CreateTexture(menu->renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STATIC, width, height);

  SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(uint32_t));
  free(pixels);
}
