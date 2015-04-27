#include "mmp.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test mmp_folder\n");
  mmp_t *mmp = mmp_init(argv[1]);

  mmp_cleanup(mmp);
}
