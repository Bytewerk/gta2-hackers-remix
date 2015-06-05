#include "sfx.h"

int main() {

  if (SDL_Init(SDL_INIT_AUDIO))
    exit(printf("SDL ERROR: %s\n", SDL_GetError()));

  sfx_t *sfx = sfx_init();

  // sfx_play(sfx, SFX_FSTYLE_CHEAT);

  Mix_Init(0);
  Mix_AllocateChannels(2);

  Mix_Music *song = Mix_LoadMUS("GTA2/data/GTAudio/D.wav");
  if (!song)
    exit(printf("Mix_LoadWAV: %s\n", Mix_GetError()));

  Mix_PlayMusic(song, -1);

  printf("sleeping 10s...\n");
  SDL_Delay(10000);

  Mix_FreeMusic(song);
  Mix_Quit();

  sfx_cleanup(sfx);
  SDL_Quit();
}
