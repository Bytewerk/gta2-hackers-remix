#include "sfx.h"
#include <stdio.h>

void sfx_load_song(sfx_t *sfx, const char *path) {
  Mix_Music *music = Mix_LoadMUS(path);
  if (!music) {
    printf("WARNING: failed to load song '%s'!\n", path);
    printf("\t(%s)\n", Mix_GetError());
    return;
  }

  sfx_song_t *song = malloc(sizeof(sfx_song_t));
  song->music = music;
  song->filename = path;

  // append to the head of the list
  song->next = sfx->songs ? sfx->songs : NULL;
  sfx->songs = song;
}

sfx_t *sfx_init() {
  sfx_t *sfx = calloc(1, sizeof(sfx_t));

  Mix_Init(0);
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
  Mix_AllocateChannels(1);

  // load all songs
  sfx_load_song(sfx, SFX_SONG_MAINMENU);
  sfx_load_song(sfx, SFX_SONG_CREDITS);

  // load all menu samples
  sfx->sdt = sfx_sdt_load("GTA2/data/audio/", "fstyle");

  return sfx;
}

void sfx_play_song(sfx_t *sfx, const char *filename) {
  sfx_song_t *current = sfx->songs;
  while (current) {
    if (!strcmp(current->filename, filename))
      break;
    current = current->next;
  }
  if (!current) {
    printf("NOTE: Failed to play song '%s' (it's not loaded!)\n", filename);
    return;
  }

  Mix_PlayMusic(current->music, -1);
}

void sfx_play_sample(sfx_t *sfx, uint8_t sample_id) {
  Mix_PlayChannel(0, sfx->sdt->chunks[sample_id], 0);
}

void sfx_cleanup(sfx_t *sfx) {
  // free all songs
  sfx_song_t *current = sfx->songs;
  while (current) {
    sfx_song_t *old = current;
    current = old->next;
    Mix_FreeMusic(old->music);
    free(old);
  }

  sfx_sdt_cleanup(sfx->sdt);
  free(sfx);
  Mix_CloseAudio();
  Mix_Quit();
}
