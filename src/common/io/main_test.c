#include "io.h"
#include <stdio.h>
#include <stdlib.h>

void callback(char *filename, void *userdata) {
  int *counter = (int *)userdata;
  *counter = *counter + 1;

  uint16_t size;
  char *buffer = io_load_small_file_to_ram(filename, &size, 0);
  printf("\tcounter: %i\n", *counter);
  printf("\tsize: %i\n", size);
  free(buffer);
}

int main(int argc, const char **argv) {
  int counter = 0;

  io_iterate_over_files_in_folder("../../../GTA2/data/frontend", "tga",
                                  callback, &counter, 0);
}
