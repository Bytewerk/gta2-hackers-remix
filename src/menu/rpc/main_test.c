#include "rpc.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  char *pref_path = SDL_GetPrefPath("bytewerk", "G2HR");
  printf("pref_path: %s\n", pref_path);
  rpc_init(pref_path);
  free(pref_path);

  printf("done!\n");
}
