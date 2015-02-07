#include "stdio.h"
#include "toolkit.h"

/*
        This file is a stub! It should load GTA2 sound files to
        some struct that SDL can play.

        The first approach was, to create a wav file, and then load
        that wav file with SDL. However, that would require writing
        it to the harddrive again. There's probably a better way!


        New approach (not programmed yet):
                Create an internal struct to store the audio data (meta + raw
   audio).

                Create an own SDL_AudioSpec
                        http://wiki.libsdl.org/SDL_AudioSpec
                ...and open the default output device:
                        http://wiki.libsdl.org/SDL_OpenAudioDevice#device
                        (allow_changes to 0, so SDL takes care of transforming
   the format, if necessary)

                Fill it with audio data, when it should play a sound:
                        http://wiki.libsdl.org/SDL_ClearQueuedAudio
                        http://wiki.libsdl.org/SDL_QueueAudio

        - Oh wait, that won't work, because then we can't play background music
   in the menu (wav format).
                So we'll need to use the SDL2_mixer library for that I guess.
        - Loading RAW audio data into SDL2_mixer seems to be complicated as
   well. We could also
                play the menu music in the native program (hack hack), so the
   approach above is good enough.
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
  // and save the ones we need in RAM
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
  }
  fclose(raw);
  fclose(sdt);
}
