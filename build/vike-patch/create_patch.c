#include "../../src-3rdparty/bytestring-delta/bdelta.h"
#include "../../src/common/fs/fs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3)
    exit(printf("Syntax: %s gta2.exe gta2_vike.exe\n", argv[0]));

  size_t size_patch;
  uint32_t size_old;
  uint32_t size_new;

  void *buffer_patch;
  char *buffer_old = fs_load_small_file_to_ram(argv[1], &size_old, 0);
  char *buffer_new = fs_load_small_file_to_ram(argv[2], &size_new, 0);

  printf("starting patch...\n");
  BDELTAcode code = bdelta_diff(buffer_old, size_old, buffer_new, size_new,
                                &buffer_patch, &size_patch);
  printf("patch result: %i\n", code);

  /*
          TODO: write the patch file and verify!
  */

  free(buffer_old);
  free(buffer_new);
  free(buffer_patch);
}
