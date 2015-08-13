#include "../../src-3rdparty/xz-embedded/xz.h"
#include "packed_files.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

int main() {
  xz_crc32_init();
  xz_dec_t *xz_dec = xz_dec_init(XZ_SINGLE, 1024 * 64);

  char **pos = PACKED_FILENAMES;
  int i = 0;
  while (**pos) {
    printf("%s\n", *pos);
    extract_file(xz_dec, i, "README.md");
    pos++;
    i++;

    break; // debug
  }

  xz_dec_end(xz_dec);
}
