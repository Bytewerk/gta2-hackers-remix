#include "../../src-3rdparty/bytestring-delta/bdelta.h"
#include "../../src/common/fs/fs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 4)
    exit(printf("Syntax: %s gta2.exe gta2_vike.exe"
                " patch_output\n",
                argv[0]));

  // load the input files
  size_t size_patch;
  uint32_t size_old;
  uint32_t size_new;

  void *buffer_patch;
  char *buffer_old = fs_load_small_file_to_ram(argv[1], &size_old, 0);
  char *buffer_new = fs_load_small_file_to_ram(argv[2], &size_new, 0);

  printf("starting patch...\n");
  BDELTAcode code = bdelta_diff(buffer_old, size_old, buffer_new, size_new,
                                &buffer_patch, &size_patch);
  if (code)
    exit(printf("ERROR: bdelta failed with code %i!\n", code));

  printf("writing to %s...\n", argv[3]);
  FILE *handle = fopen(argv[3], "wb");
  if (!handle)
    exit(printf("ERROR: failed to open the file!\n"));
  fwrite(buffer_patch, size_patch, 1, handle);
  fclose(handle);

  printf("cleaning up...\n");
  free(buffer_old);
  free(buffer_new);
  free(buffer_patch);
}
