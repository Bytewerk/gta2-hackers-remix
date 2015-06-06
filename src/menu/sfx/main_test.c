#include "sfx.h"

int main() {

  if (SDL_Init(SDL_INIT_AUDIO))
    exit(printf("SDL ERROR: %s\n", SDL_GetError()));

  sfx_t *sfx = sfx_init();

  // FIXME: this is just playing silence right now!
  for (int i = 0; i < sfx->sdt->count; i++) {
    printf("playing sample #%i...\n", i);
    if (Mix_PlayChannel(0, sfx->sdt->chunks[i], 0) == -1)
      printf("Mix_PlayChannel: %s\n", Mix_GetError());

    // wait until it is finished
    while (Mix_Playing(0))
      SDL_Delay(100);
  }

  /*
  sfx_play_song(sfx, SFX_SONG_MAINMENU);
  SDL_Delay(4000);
  sfx_play_song(sfx, SFX_SONG_CREDITS);
  SDL_Delay(4000);
  */

  sfx_cleanup(sfx);
  SDL_Quit();
}
