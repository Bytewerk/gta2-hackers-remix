#include "screen_layout.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test path/to/screen_layouts.cfg\n");

  sl_t *sl = sl_init((char *)argv[1]);
  sl_cleanup(sl);

  printf("cleaned up!\n");
}
