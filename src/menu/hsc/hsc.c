#include "hsc.h"
#include "../../common/io/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

hsc_t *hsc_load(hsc_t *hsc, char *filename) {
  if (!hsc)
    hsc = calloc(1, sizeof(hsc_t));
  if (!access(filename, 0)) {
    printf("NOTE: couldn't read high scores, setting default"
           " values (STUB)...\n");
    return hsc;
  }

  uint16_t size;
  char *buffer = io_load_small_file_to_ram(filename, &size, 0);

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
