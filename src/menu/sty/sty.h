#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

/*
    The following structs are more or less directly
    from the official spec ("GTA2 Style Format.doc"):
        font_base_t
        sprite_meta_t
        pallete_base_t
        sprite_base_t
*/

typedef struct {
  uint16_t font_count;
  uint16_t *base;
} font_base_t;

// this contains info about a sprite, but not
// the sprite itself!
typedef struct {
  uint32_t ptr;
  uint8_t width;
  uint8_t height;
  uint16_t pad;
} sprite_meta_t;

typedef struct {
  uint16_t count;
  sprite_meta_t *entries;
} sprite_index_t;

typedef struct {
  uint32_t blob_length;
  char *blob;
} sprite_blob_t;

typedef struct {
  uint16_t tile;
  uint16_t sprite;
  uint16_t car_remap;
  uint16_t ped_remap;
  uint16_t code_obj_remap;
  uint16_t map_obj_remap;
  uint16_t user_remap;
  uint16_t font_remap;
} pallete_base_t;

typedef struct {
  uint16_t car;
  uint16_t ped;
  uint16_t code_obj;
  uint16_t map_obj;
  uint16_t user;
  uint16_t font;
} sprite_base_t;

typedef struct {
  font_base_t font_base;
  sprite_index_t sprite_index;
  sprite_blob_t sprite_blob;
  sprite_base_t sprite_base;
  pallete_base_t pallete_base;
  uint16_t pallete_index[16384];
  uint32_t pallete[64 * 64 * 256];
} sty_t;

sty_t *sty_load(char *filename);
void sty_cleanup(sty_t *sty);
SDL_Texture *sty_sprite(SDL_Renderer *renderer, sty_t *sty, char silent,
                        int sprite_id);
void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, SDL_Rect dest,
              char *text);
