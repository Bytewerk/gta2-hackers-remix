#include "../../common/cstr/cstr.h"
#include "../../common/fs/fs.h"
#include "sfx.h"
#include <stdio.h>

// based on Black_Phoenix' sdt2wav tool: http://git.io/g2hr-std2wav
typedef struct {
  int32_t start_offset;
  int32_t size;
  int32_t freq;

  int32_t unknown;
  int32_t loop_start;
  int32_t loop_size;
} sfx_sdt_chunk_t;

// based on Black_Phoenix' sdt2wav tool: http://git.io/g2hr-std2wav
typedef struct {
  int32_t riff;
  int32_t header_size;
  int32_t wave;
  int32_t fmt;
  int32_t fixed1;
  int16_t format_tag;
  int16_t channels;
  int32_t sample_rate;
  int32_t bytes_per_second;
  int16_t bytes_per_sample;
  int16_t bits_per_sample;
  int32_t data; //?
  int32_t audio_data_size;
  int32_t size;
} sfx_sdt_wave_header_t;

// based on Black_Phoenix' sdt2wav tool: http://git.io/g2hr-std2wav
//
// path: must have a trailing slash
// name: the filename, without the extensions (we load 2 files!)
sfx_sdt_t *sfx_sdt_load(const char *path, const char *name) {
  // load the SDT and RAW file
  uint32_t size_raw;
  uint32_t size_sdt;
  char *path_raw = cstr_merge(path, name, ".raw");
  char *path_sdt = cstr_merge(path, name, ".sdt");
  char *raw = fs_load_small_file_to_ram(path_raw, &size_raw, 0);
  sfx_sdt_chunk_t *sdt =
      (sfx_sdt_chunk_t *)fs_load_small_file_to_ram(path_sdt, &size_sdt, 0);

  // create the struct we'll return
  sfx_sdt_t *ret = malloc(sizeof(sfx_sdt_t));
  ret->count = size_sdt / 24;
  ret->chunks = malloc(sizeof(Mix_Chunk *) * ret->count);

  // split the RAW file into WAV files for easier loading with
  // SDL2_mixer
  for (int i = 0; i < ret->count; i++) {
    sfx_sdt_chunk_t *meta = &sdt[i];
    sfx_sdt_wave_header_t *wav =
        malloc(sizeof(sfx_sdt_wave_header_t) + meta->size);
    wav->riff = 0x46464952;
    wav->header_size = meta->size + 36;
    wav->wave = 0x45564157;
    wav->fmt = 0x20746D66;
    wav->fixed1 = 16;
    wav->format_tag = 1;
    wav->channels = 1; // mono
    wav->sample_rate = meta->freq;
    wav->bytes_per_second = meta->freq * 2;
    wav->bytes_per_sample = 2;
    wav->bits_per_sample = 16;
    wav->data = 0x61746164;
    wav->size = meta->size;

    // copy the raw audio data after the header (wav+1!)
    memcpy(wav + 1, raw + meta->start_offset, meta->size);

    ret->chunks[i] = Mix_QuickLoad_WAV((uint8_t *)wav);
  }

  // finish up
  free(raw);
  free(sdt);
  free(path_raw);
  free(path_sdt);
  return ret;
}

void sfx_sdt_cleanup(sfx_sdt_t *sdt) {
  for (int i = 0; i < sdt->count; i++)
    Mix_FreeChunk(sdt->chunks[i]);

  free(sdt->chunks);
  free(sdt);
}
