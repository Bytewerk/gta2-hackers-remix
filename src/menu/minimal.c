#include "bg/background.h"
#include "sfx/sfx.h"
#include "sty/sty.h"
#include "tk/toolkit.h"
#include "ui/interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

/*
        this is a minimal version of main.c to test the code with valgrind
        ('make valgrind').

        000  fsty
        032  bg (IMG_Init + PNG)
        864  sfx (SDL_OpenAudioDevice)

        -----------------
        896  total

*/

int main(int argc, char *argv[]) {
  // one TGA, one PNG is enough
  const char *tgas[] = {"1", "g2hr_splitscreen"};

  // init functions
  IMG_Init(IMG_INIT_PNG); // 32b
  sty_t *fsty = sty_load("data/fstyle.sty");
  bg_t *bg = bg_init(tgas, sizeof(tgas) / sizeof(char *));
  sfx_t *sfx = sfx_init(); // 864b

  // cleanup functions
  sfx_cleanup(sfx);
  sty_cleanup(fsty);
  bg_cleanup(bg);
  IMG_Quit();
  SDL_Quit();

  printf("cleaned up!\n");
  return 0;
}
