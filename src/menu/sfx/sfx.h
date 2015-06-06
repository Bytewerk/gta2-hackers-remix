#pragma once
#include "songs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct sfx_song_t {
  struct sfx_song_t *next;
  Mix_Music *music;
  const char *filename;
} sfx_song_t;

typedef struct {
  sfx_song_t *songs;
  // TODO: samples
} sfx_t;

sfx_t *sfx_init();

// filenames are in songs.h!
void sfx_play_song(sfx_t *sfx, const char *filename);

void sfx_cleanup(sfx_t *sfx);
