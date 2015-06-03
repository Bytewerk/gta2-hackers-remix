#include "io.h"
#include <stdio.h>
#include <stdlib.h>

void callback(char *path, char *name, char *ext, void *userdata) {
  int *counter = (int *)userdata;
  *counter = *counter + 1;

  printf("file #%i:\n", *counter + 1);
  printf("\tpath: %s\n", path);
  printf("\tname: %s\n", name);
  printf("\text:  %s\n", ext);

  uint32_t size;
  char *buffer = fs_load_small_file_to_ram(path, &size, 1);
  printf("\tsize: %i\n", size);
  free(buffer);
}

int main(int argc, const char **argv) {
  int counter = 0;

  fs_iterate_over_files_in_folder("../../../GTA2/data/frontend", "tga",
                                  callback, &counter, 0);
}
