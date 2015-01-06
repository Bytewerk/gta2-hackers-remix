#include "sty_parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// offsets in fstyle.sty
// PPAL: 000006
// PALX: 065608
// PALB: 767888

// FIXME
// returns the next offset or 0
uint32_t sty_parser_read_next_chunk(char *buffer, uint32_t offset,
                                    uint32_t sty_size) {
  // read the chunk type and size
  char type[5];
  for (int i = 0; i < 4; i++)
    type[i] = buffer[offset + i];
  type[5] = '\0';
  uint32_t size = (*(uint32_t *)(buffer + offset + 5)) + 8;

  printf("Chunk: %s (size: %i)\n", type, size);
  printf("sty size:    %i\n", sty_size);
  printf("Offset:      %i\n", offset);
  printf("Offset+size: %i\n", offset + size);
  printf("\n");

  offset += size;
  if (!size || offset >= sty_size)
    exit(printf("ERROR!\n"));
  return offset;
}

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

  uint32_t offset = 6; // skip the header!
  uint32_t sty_parser_read_next_chunk(char *buffer, uint32_t offset,
                                      uint32_t sty_size);

  while (offset)
    offset = sty_parser_read_next_chunk(buffer, offset, size);

  return NULL;
}
