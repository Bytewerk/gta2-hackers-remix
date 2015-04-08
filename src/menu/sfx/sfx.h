#pragma once
#include <SDL2/SDL.h>

/*
        These are the IDs of the GTA2 sounds in fstyle.raw.
        Lots of files sound the same (eg. 0-3), although they
        *are* different (md5'd them) so we'll just pick the first
        one in that case.

        In total, we have 17 sound samples in there. Use the sdt2wav
        tool by Black_Phoenix to convert them to WAV and listen to them
        independently.

        All of them need to be verified by directly comparing when the
        original GTA2 menu is opened.
*/
#define SFX_FSTYLE_RETURN 0
#define SFX_FSTYLE_LEFT 8   // not sure
#define SFX_FSTYLE_RIGHT 10 // not sure
#define SFX_FSTYLE_BACK 12
#define SFX_FSTYLE_CHEAT 16
#define SFX_FSTYLE_TOTAL_COUNT 18

typedef struct {
  uint32_t byte_count;
  void *raw;
} sfx_sample_t;

typedef struct {
  sfx_sample_t *sample[SFX_FSTYLE_TOTAL_COUNT];
  SDL_AudioDeviceID dev;
} sfx_t;

sfx_t *sfx_init();
void sfx_play(sfx_t *sfx, int id);
void sfx_cleanup(sfx_t *sfx);
