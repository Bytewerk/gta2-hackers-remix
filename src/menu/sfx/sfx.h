#pragma once
#include "samples.h"
#include "sdt.h"
#include "songs.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct sfx_song_t {
  struct sfx_song_t *next;
  Mix_Music *music;
  const char *filename;
} sfx_song_t;

typedef struct {
  sfx_song_t *songs;
  sfx_sdt_t *sdt;
  bool sound_works;
} sfx_t;

sfx_t *sfx_init();

// filenames are in songs.h!
void sfx_play_song(sfx_t *sfx, const char *filename);

// sample ids are in samples.h!
void sfx_play_sample(sfx_t *sfx, uint8_t sample_id);

void sfx_cleanup(sfx_t *sfx);
