#include "packed_files.h"
#include <stdio.h>

int main() {
  char **pos = PACKED_FILENAMES;
  while (**pos) {
    printf("%s\n", *pos);
    pos++;
  }
}
