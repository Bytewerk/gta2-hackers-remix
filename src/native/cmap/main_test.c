#include "cmap.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  cmap_t *cmap = cmap_init();

  printf("cleaning up...\n");
  cmap_cleanup(cmap);
}
