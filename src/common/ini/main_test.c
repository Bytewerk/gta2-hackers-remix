#include "ini.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test ini_file\n");

  ini_t *ini = ini_open(argv[1], 0);

  printf("cleaning up...\n");

  ini_cleanup(ini);

  printf("done!\n");
}
