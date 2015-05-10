#include "rpc.h"
#include "../../common/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RPC_ORIGINAL_LENGTH 21
void rpc_replace(char *exe_buffer, size_t size, char *cache_file, int i) {
  printf("writing %s...\n", cache_file);

  char *original = "\\DMA Design Ltd\\GTA2\\";
  char replaced[RPC_ORIGINAL_LENGTH + 1];
  sprintf(replaced, "\\GTA2HackersRemix\\P%i\\", i + 1);

  FILE *handle = fopen(cache_file, "w");

  for (int i = 0; i < size; i++) {
    // check if this is the first character of the 'original' string
    char is_start_of_original = 1;
    for (int j = 0; i + j < size && j < RPC_ORIGINAL_LENGTH; j++)
      if (exe_buffer[i + j] != original[j]) {
        is_start_of_original = 0;
        break;
      }

    // replace the original string
    if (is_start_of_original && (i + RPC_ORIGINAL_LENGTH) < size) {
      i += RPC_ORIGINAL_LENGTH - 1;
      fputs(replaced, handle);
    }

    // otherwise just put the character there
    else
      fputc(exe_buffer[i], handle);
  }

  fclose(handle);
}

#define RPC_CACHE_PATH_LENGTH 1000
void rpc_init(char *prefpath) {
  char cache_file[RPC_CACHE_PATH_LENGTH + 1];
  char *exe_buffer = NULL;
  size_t size;

  for (int i = 0; i < GTA2_PLAYER_COUNT; i++) {
    snprintf(cache_file, RPC_CACHE_PATH_LENGTH, "%sG2HR_PLAYER%i.exe", prefpath,
             i + 1);
    if (!access(cache_file, 0))
      continue;

    // read GTA2.exe, if that hasn't been done already
    if (!exe_buffer) {
      char *gta2_exe = "GTA2/gta2.exe";
      FILE *handle = fopen(gta2_exe, "rb");
      if (!handle)
        exit(printf("ERROR: Couldn't read '%s'!\n", gta2_exe));

      // calculate file size
      fseek(handle, 0, SEEK_END);
      size = ftell(handle);

      // read the whole file into RAM (<<10 MB) and close it
      rewind(handle);
      exe_buffer = (char *)malloc(size);
      if (fread(exe_buffer, 1, size, handle) != size)
        exit(printf("Read error while reading '%s'!\n", gta2_exe));
      fclose(handle);
    }

    // perform search and replace
    rpc_replace(exe_buffer, size, cache_file, i);
  }

  if (exe_buffer)
    free(exe_buffer);
}
#undef RPC_CACHE_PATH_LENGTH
