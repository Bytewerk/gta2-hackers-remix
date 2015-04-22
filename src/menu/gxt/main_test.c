#include "gxt.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test gta2_text_file\n");
  gxt_t *gxt = gxt_load(argv[1]);

  gxt_cleanup(gxt);
}
