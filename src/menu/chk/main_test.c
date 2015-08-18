#include "chk.h"
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
  SDLNet_Init();
  chk_t *chk = chk_init("./", false);
  chk_cleanup(chk);
  SDLNet_Quit();
}
