#include "chk.h"
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
  SDLNet_Init();
  chk_t *chk = chk_init(".");

  while (1) {
    SDL_Delay(100);
    if (chk->is_version_set)
      break;
  }

  printf("version: %s\n", chk->latest_version);

  chk_cleanup(chk);

  SDLNet_Quit();
}
