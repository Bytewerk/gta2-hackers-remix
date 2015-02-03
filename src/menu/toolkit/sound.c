#include "stdio.h"
#include "toolkit.h"

/*
        This file is a stub! It should load GTA2 sound files to
        some struct that SDL can play.

        The first approach was, to create a wav file, and then load
        that wav file with SDL. However, that would require writing
        it to the harddrive again. There's probably a better way!

*/

/*
        Based on Black_Phoenix' sdt2wav tool:
                http://git.io/g2hr-sdt2wav
*/
void tk_init_gta2_sound(tk_t *tk) {
  tk_sound_t *sound = malloc(sizeof(tk_sound_t));

  // open both files (the bigger 'raw' file contains the real
  // audio, the 'sdt' contains meta data)
  printf("loading data/fstyle.sdt...");
  FILE *raw = fopen("data/fstyle.raw", "rb");
  FILE *sdt = fopen("data/fstyle.sdt", "rb");
  if ((!raw) || (!sdt))
    exit(printf("File read error!\n"));

  // find out the number of audio files included by the size of
  // the 'sdt' file
  int fileid = 0;
  fseek(sdt, 0, 2);
  int fmax = ftell(sdt) / 24;
  fseek(sdt, 0, 0);

  // iterate through all included audio samples of the raw file,
  // and save the ones we need in the WAV format in RAM
  for (int i = 0; i < fmax; i++) {
    // TODO: check if we actually need this file, otherwise continue!

    // read meta data of current sample
    int start_offset, size, freq, unk1;
    unsigned short unk2, unk3;
    fread(&start_offset, 4, 1, sdt);
    fread(&size, 4, 1, sdt);
    fread(&freq, 4, 1, sdt);
    fread(&unk1, 4, 1, sdt);
    fread(&unk2, 4, 1, sdt);
    fread(&unk3, 4, 1, sdt);

    // SDL2 can load wav files directly, so we'll create one in RAM
    // TODO: SDL2 can load wav files only from files! we'll need to directly
    // push the audio into SDL structs!
    /*
    tk_sdt2wav_header_t* wav = malloc(sizeof(tk_sdt2wav_header_t) + size);
    wav->riff_chunk_id = 0x46464952; // RIFF
    wav->riff_chunk_size = size + 36;
    wav->riff_type = 0x45564157; // WAVE
    wav->fmt_chunk_id = 0x20746D66; // "fmt "
    wav->fmt_16 = 16;
    wav->fmt_format_tag = 1;
    wav->fmt_channels = 1;
    wav->fmt_sample_rate = freq;
    wav->fmt_bytes_per_second = freq * 2;
    wav->fmt_bytes_per_sample = 2;
    wav->fmt_significant_bits = 16;
    wav->fmt_extra_format_bytes = 0x61746164;
    wav->fmt_audio_data_size = size;

    // add the sound data after the header
    fseek(raw,start_offset,0);
    fread(wav + sizeof(tk_sdt2wav_header_t),size,1,raw);
    */

    // TODO: actually import with SDL, append to tk struct!
    // TODO: also write cleanup function!!

    // free(wav);
  }
  fclose(raw);
  fclose(sdt);
}
