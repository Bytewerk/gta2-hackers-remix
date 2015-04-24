#include "sfx.h"
#include "stdio.h"
#include <SDL2/SDL.h>

// This function is heavily based on Black_Phoenix' sdt2wav tool:
// http://git.io/g2hr-sdt2wav
// FIXME: Code will probably be nicer when reading the metadata into ram first
// (we could skip all the freads for stuff we don't need then)
void sfx_load_fstyle(sfx_t *sfx) {
  // open both files (the bigger 'raw' file contains the real
  // audio, the 'sdt' contains meta data)
  printf("loading GTA2/data/audio/fstyle.sdt...\n");
  FILE *raw = fopen("GTA2/data/audio/fstyle.raw", "rb");
  FILE *sdt = fopen("GTA2/data/audio/fstyle.sdt", "rb");
  if ((!raw) || (!sdt))
    exit(printf("File read error!\n"));

  // calculate the sample count from the 'sdt' file size
  // and verify it
  fseek(sdt, 0, 2);
  int samples_count = ftell(sdt) / 24;
  fseek(sdt, 0, 0);
  if (samples_count != SFX_FSTYLE_TOTAL_COUNT)
    exit(printf("ERROR: Wrong sample count in fstyle.sdt (%i, expected: %i)!\n",
                samples_count, SFX_FSTYLE_TOTAL_COUNT));

  // iterate through all included audio samples of the raw file,
  // and save the ones we need in the sfx struct
  for (int i = 0; i < samples_count; i++) {
    // check if we need this sample at all
    if (i != SFX_FSTYLE_RETURN && i != SFX_FSTYLE_LEFT &&
        i != SFX_FSTYLE_RIGHT && i != SFX_FSTYLE_BACK && i != SFX_FSTYLE_CHEAT)
      continue;

    // read meta data
    int start_offset, size, freq, unk1;
    unsigned short unk2, unk3;
    fread(&start_offset, 4, 1, sdt);
    fread(&size, 4, 1, sdt);
    fread(&freq, 4, 1, sdt);
    fread(&unk1, 4, 1, sdt);
    fread(&unk2, 4, 1, sdt);
    fread(&unk3, 4, 1, sdt);

    // save the sample to the sfx struct
    sfx_sample_t *sample = malloc(sizeof(sfx_sample_t));
    sample->raw = malloc(size);
    fseek(raw, start_offset, 0);
    fread(sample->raw, size, 1, raw);
    sample->byte_count = size;
    sfx->sample[i] = sample;
  }

  fclose(raw);
  fclose(sdt);
}

sfx_t *sfx_init() {
  // Allocate memory for our struct that will be passed
  // to all other functions
  sfx_t *sfx = malloc(sizeof(sfx_t));
  for (int i = 0; i < SFX_FSTYLE_TOTAL_COUNT; i++)
    sfx->sample[i] = NULL;

  // Open the default audio device with the GTA2 sound format
  SDL_AudioSpec want, have;
  SDL_zero(want);
  want.freq = 22050;
  want.format = AUDIO_S16LSB; // not sure
  want.channels = 1;
  want.samples = 4096;
  sfx->dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

  // load all samples
  sfx_load_fstyle(sfx);

  return sfx;
}

// Just cut off already playing SFX. GTA2 does it the same way.
void sfx_play(sfx_t *sfx, int id) {
  // FIXME: These functions don't exist yet, we'll have to wait for SDL 2.0.4!
  // sfx_sample_t* sample = sfx->sample[id];
  // SDL_ClearQueuedAudio(sfx->dev);
  // SDL_QueueAudio(sfx->dev, sample->raw, sample->byte_count);

  printf("playing sample #%i\n", id); // debug
}

void sfx_cleanup(sfx_t *sfx) {
  // free all samples first
  for (int i = 0; i < SFX_FSTYLE_TOTAL_COUNT; i++) {
    if (!sfx->sample[i])
      continue;
    free(sfx->sample[i]->raw);
    free(sfx->sample[i]);
  }

  SDL_CloseAudioDevice(sfx->dev);
  free(sfx);
}
