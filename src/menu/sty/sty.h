#pragma once
#include <stdint.h>

/*
    The following structs are more or less directly
    from the official spec ("GTA2 Style Format.doc"):
        font_base_t
        sprite_meta_t
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
  uint16_t sprite_count;
  sprite_meta_t *sprite_entries;
} sprite_index_t;

typedef struct {
  uint32_t blob_length;
  char *blob;
} sprite_blob_t;

typedef struct {
  font_base_t font_base;
  sprite_index_t sprite_index;
  sprite_blob_t sprite_blob;
} sty_t;

sty_t *sty_load(char *filename);
void sty_cleanup(sty_t *sty);
