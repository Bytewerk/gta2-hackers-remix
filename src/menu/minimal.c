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
  sty_t *fsty = sty_load("data/fstyle.sty");
  sty_cleanup(fsty);
  printf("cleaned up!\n");
  return 0;
}
