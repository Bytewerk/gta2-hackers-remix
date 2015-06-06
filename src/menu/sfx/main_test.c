#include "sfx.h"

int main() {

  if (SDL_Init(SDL_INIT_AUDIO))
    exit(printf("SDL ERROR: %s\n", SDL_GetError()));

  sfx_t *sfx = sfx_init();

  sfx_play_song(sfx, SFX_SONG_MAINMENU);
  SDL_Delay(4000);
  sfx_play_song(sfx, SFX_SONG_CREDITS);
  SDL_Delay(4000);

  sfx_cleanup(sfx);
  SDL_Quit();
}
