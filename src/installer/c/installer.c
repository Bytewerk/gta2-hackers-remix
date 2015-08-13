#include "../../../src-3rdparty/xz-embedded/xz.h"
#include "packed_files.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct xz_dec xz_dec_t;
typedef struct xz_buf xz_buf_t;
typedef enum xz_ret xz_ret_t;

void extract_file(xz_dec_t *xz_dec, uint16_t index, char *output) {
  char *start = PACKED_START[index];
  char *end = PACKED_END[index];
  size_t size = PACKED_UNCOMPRESSED_SIZE[index];
  char *buffer = malloc(size);

  xz_buf_t xz_buf = {(uint8_t *)start,  0, end - start,

                     (uint8_t *)buffer, 0, size};
  xz_dec_run(xz_dec, &xz_buf);

  FILE *handle = fopen(output, "wb");
  fwrite(buffer, size, 1, handle);
  fclose(handle);
  free(buffer);
}

uint16_t get_index(char *name) {
  uint16_t i = 0;
  for (char **pos = PACKED_FILENAMES; **pos; pos++) {
    if (!strcmp(name, *pos))
      return i;
    i++;
  }
  printf("ERROR: couldn't find the index for %s!\n", name);
  return 0;
}

int main() {
  xz_crc32_init();
  xz_dec_t *xz_dec = xz_dec_init(XZ_SINGLE, 0);
  extract_file(xz_dec, get_index("bin/g2hr.exe"), "g2hr.exe");
  xz_dec_end(xz_dec);
}
