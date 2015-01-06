#include "sty_parser.h"
#include <stdio.h>
#include <stdlib.h>

sty_t *sty_parser(char *filename) {
  printf("sty_parser: reading file %s\n", filename);

  // open the file
  FILE *handle;
  handle = fopen(filename, "rb");
  if (handle == NULL)
    exit(printf("Can't open the file!\n"));

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(sizeof(char) * size);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error!\n"));
  fclose(handle);

  if (buffer[0] != 'G' || buffer[1] != 'B' || buffer[2] != 'S' ||
      buffer[3] != 'T')
    exit(printf("This isn't a GBST file!\n"));

  // ...

  return NULL;
}
