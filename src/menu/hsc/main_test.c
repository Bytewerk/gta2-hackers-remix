#include "hsc.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test hiscore_file\n");
  hsc_t *hsc = hsc_load(NULL, argv[1]);

  for (int i = 0; i < 5; i++) {
    printf("%i: %8s %i\n", i, hsc->level[0].entry[i].name,
           hsc->level[0].entry[i].score);
  }
}
