#include "hsc.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test hiscore_file\n");
  hsc_t *hsc = hsc_load(NULL, argv[1]);

  for (int i = 0; i < GTA2_LEVEL_COUNT; i++) {
    printf("------ level %i -------\n", i);
    for (int j = 0; j < 5; j++) {
      printf("%i: %8s %i\n", j, hsc->level[0].entry[j].name,
             hsc->level[i].entry[j].score);
    }
  }
}
