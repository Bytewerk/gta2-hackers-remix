#include "../../../src-3rdparty/xz-embedded/xz.h"
#include "../../common/cstr/cstr.h"
#include "packed_files.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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

  printf("extracting '%s' to '%s'...\n", PACKED_FILENAMES[index], output);
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

int main(int argc, char **argv) {
  char temp[200];
  GetTempPath(199, temp);

  // default action: extract g2hr.exe and run the autoit installer,
  // which will show a nice gui and use this installer exe-file to
  // extract all other files, once the setup is configured.
  if (argc == 1) {
    char *gui = cstr_merge(temp, "\\g2hr_installer_gui.exe");
    xz_crc32_init();
    xz_dec_t *xz_dec = xz_dec_init(XZ_SINGLE, 0);
    extract_file(xz_dec, get_index("bin/g2hr.exe"), gui);

    char *cmd = cstr_merge(gui, " install");
    printf("%s\n", cmd);
    system(cmd);

    // clean up
    xz_dec_end(xz_dec);
    free(gui);
    free(cmd);
  }
}
