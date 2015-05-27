#include "chk.h"
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
  SDLNet_Init();
  update_check_in_background(".");
  SDLNet_Quit();
}
