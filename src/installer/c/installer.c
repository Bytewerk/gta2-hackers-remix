#include "installer.h"
#include "../../../src-3rdparty/mbedtls-sha512/sha512.h"
#include "../../../src-3rdparty/xz-embedded/xz.h"
#include "../../common/cstr/cstr.h"
#include "packed_files.h"
#include <stdbool.h>
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
  if (!handle)
    exit(printf("failed to open output file!\n"));
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

int verify_hash(char *filename) {
  // open the file
  FILE *handle = fopen(filename, "rb");
  if (!handle)
    return printf("DONE: Couldn't read the file!\n");
  fseek(handle, 0, SEEK_END);
  if (ftell(handle) != GTA2_INSTALLER_SIZE) {
    printf("DONE: File size does not match!\n");
    fclose(handle);
    return 1;
  }
  rewind(handle);

  // prepare the sha512 library
  unsigned char hash[64];
  mbedtls_sha512_context ctx;
  mbedtls_sha512_init(&ctx);
  mbedtls_sha512_starts(&ctx, 0);

  // hash the file chunk by chunk
  char *buffer = malloc(HASH_CHUNK_SIZE);
  size_t buffer_len = HASH_CHUNK_SIZE;
  for (int i = 0; buffer_len == HASH_CHUNK_SIZE; i++) {
    buffer_len = fread(buffer, 1, HASH_CHUNK_SIZE, handle);
    mbedtls_sha512_update(&ctx, (const unsigned char *)buffer, buffer_len);

    if (i % 3 == 0)
      printf("PERCENT: %i\n",
             (i * HASH_CHUNK_SIZE) / (GTA2_INSTALLER_SIZE / 100));
  }

  // finish up
  free(buffer);
  fclose(handle);
  mbedtls_sha512_finish(&ctx, hash);
  mbedtls_sha512_free(&ctx);

  char hash_str[129];
  for (int i = 0; i < 64; i++)
    sprintf(hash_str + i * 2, "%02x", hash[i]);
  hash_str[129] = '\0';

  if (!strcmp(GTA2_INSTALLER_SHA512, hash_str))
    return printf("DONE; SHA-512 Hash verified!\n");

  printf("expected: %s\n", GTA2_INSTALLER_SHA512);
  printf("actually: %s\n", hash_str);
  printf("DONE: SHA-512 Hash could NOT be verified!\n");
  return 1;
}

// this simply tries to create every subfolder in the path
void create_folder_structure(char *path) {
  cstr_split_t *split = cstr_split(path, '\\', false);
  for (int i = 0; i < split->count - 1; i++) {
    char *partial = "";
    for (int j = 0; j <= i; j++) {
      char *old = partial;
      partial = cstr_merge(partial, split->pieces[j], "\\");
      free(old);
    }
    CreateDirectory(partial, NULL);
    free(partial);
  }
  cstr_split_free(split);
}

int extract_everything(char *target_root) {
  xz_crc32_init();
  xz_dec_t *xz_dec = xz_dec_init(XZ_SINGLE, 0);
  for (int i = 0; i < PACKED_COUNT; i++) {
    char *target = cstr_merge(target_root, "\\", PACKED_FILENAMES[i]);
    create_folder_structure(target);
    extract_file(xz_dec, i, target);
    free(target);
    printf("PERCENT: %i\n", 100 * (i + 1) / PACKED_COUNT);
  }
  xz_dec_end(xz_dec);
  printf("DONE: ALL OK\n");
  return 0;
}

int gui_installer(bool cleanup, char *installer_name) {
  char temp[200];
  GetTempPath(199, temp);
  char *gui = cstr_merge(temp, "g2hr_installer_gui.exe");
  if (cleanup) {
    Sleep(1000); // wait 1s for the gui process to quit
    DeleteFile(gui);
  } else {
    // extract the gui
    xz_crc32_init();
    xz_dec_t *xz_dec = xz_dec_init(XZ_SINGLE, 0);
    extract_file(xz_dec, get_index("bin\\g2hr.exe"), gui);
    xz_dec_end(xz_dec);

    // launch the process (yes, it is that complicated)
    char *cmd = cstr_merge(gui, " install ", installer_name);
    printf("executing: %s\n", cmd);

    STARTUPINFO *startup_info = calloc(1, sizeof(STARTUPINFO));
    PROCESS_INFORMATION *process_info = calloc(1, sizeof(PROCESS_INFORMATION));
    startup_info->cb = sizeof(STARTUPINFO);

    CreateProcess(gui, cmd, NULL, NULL, false, 0, NULL, NULL, startup_info,
                  process_info);

    free(cmd);
    free(startup_info);
    free(process_info);
  }
  free(gui);
  return 0;
}

int main(int argc, char **argv) {
  // default action: extract g2hr.exe and run the autoit installer,
  // which will show a nice gui and use this installer exe-file to
  // extract all other files, once the setup is configured.
  if (argc == 1)
    return gui_installer(false, argv[0]);

  // verify the sha512 hash of the original GTA2 installer (2nd arg)
  if (argc == 3 && !strcmp(argv[1], "verify")) {
    verify_hash(argv[2]);
    getchar(); // wait for the gui to parse stdout before exit
    return 0;
  }

  // extract everything into the given folder (2nd arg)
  if (argc == 3 && !strcmp(argv[1], "extract")) {
    extract_everything(argv[2]);
    getchar(); // wait for the gui to parse stdout before exit
    return 0;
  }

  // clean up the temp files extracted for the gui installer
  if (argc == 2 && !strcmp(argv[1], "cleanup"))
    return gui_installer(true, argv[0]);

  printf("invalid commandline parameters, see the source for more"
         " info: http://git.io/g2hr\n");
  return 1;
}
