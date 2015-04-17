#include "hsc.h"
#include <stdio.h>
#include <stdlib.h>

hsc_t *hsc_load(hsc_t *hsc, const char *filename) {
  // open the file
  printf("loading %s...\n", filename);
  if (!hsc)
    hsc = calloc(1, sizeof(hsc_t));
  FILE *handle = fopen(filename, "rb");

  // default values
  if (!handle) {
    // FIXME: magic numbers, use real gta2 default scores
    printf("Couldn't read file, setting default values...\n");
    // ...
    return hsc;
  }

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(size);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error!"));
  fclose(handle);

  for (int i = 0; i < GTA2_LEVEL_COUNT; i++) {
    for (int j = 0; j < 5; j++) // entries
    {
      // player name
      for (int k = 0; k < 16; k += 2)
        hsc->level[i].entry[j].name[k / 2] = buffer[j * 24 + k];

      // score, TODO
      hsc->level[i].entry[j].score =
          ((uint8_t)buffer[i * 24 * 5 + j * 24 + 20]) * 1000 / 8 * 5;
    }
  }

  free(buffer);
  return hsc;
}

void hsc_cleanup(hsc_t *hsc) { free(hsc); }
