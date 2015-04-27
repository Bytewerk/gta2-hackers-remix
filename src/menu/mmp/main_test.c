#include "mmp.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test mmp_file\n");
  mmp_t *mmp = mmp_load((char *)argv[1]);

  mmp_cleanup(mmp);
}
