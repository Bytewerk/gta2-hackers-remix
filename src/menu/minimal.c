#include "bg/background.h"
#include "sfx/sfx.h"
#include "sty/sty.h"
#include "tk/toolkit.h"
#include "ui/interface.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// this is a minimal version of main.c to test the code with valgrind.
// make valgrind.

int main(int argc, char *argv[]) {
  // one TGA, one PNG is enough
  const char *tgas[] = {"1", "g2hr_splitscreen"};

  sty_t *fsty = sty_load("data/fstyle.sty");
  bg_t *bg = bg_init(tgas, sizeof(tgas) / sizeof(char *));

  sty_cleanup(fsty);
  bg_cleanup(bg);

  SDL_Quit();
  printf("cleaned up!\n");
  return 0;
}
