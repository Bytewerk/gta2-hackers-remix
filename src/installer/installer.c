#include "packed_files.h"
#include <stdint.h>
#include <stdio.h>

void extract_file(uint16_t index) {}

int main() {
  char **pos = PACKED_FILENAMES;
  while (**pos) {
    printf("%s\n", *pos);
    pos++;
  }
}
