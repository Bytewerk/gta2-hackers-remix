#include "rpc.h"
#include "../../common/common.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RPC_SEARCH_STR "\\DMA Design Ltd\\GTA2\\"
void rpc_replace(char *exe_buffer, size_t size, char *cache_file,
                 int player_num, rpc_pos_t *pos) {
  printf("caching %s...\n", cache_file);

  // generate the replaced-string
  char *original = RPC_SEARCH_STR;
  int original_len = strlen(original);
  char replaced[original_len + 1];
  sprintf(replaced, "\\GTA2HackersRemix\\P%i\\", player_num + 1);

  FILE *handle = fopen(cache_file, "wb");

  size_t pos_last = 0;
  while (pos) {
    size_t pos_end = pos->pos;

    fwrite(exe_buffer + pos_last, pos_end - pos_last, 1, handle);
    fwrite(replaced, original_len, 1, handle);

    pos_last = pos->pos + original_len;
    pos = pos->next;
  }

  // write the rest, close the file
  fwrite(exe_buffer + pos_last, size - pos_last, 1, handle);
  fclose(handle);
}

rpc_pos_t *rpc_search(char *exe_buffer, size_t size) {
  rpc_pos_t *first = NULL;
  rpc_pos_t *last;

  char *original = RPC_SEARCH_STR;
  int original_length = strlen(original);

  for (size_t i = 0; i < size; i++) {
    for (int j = 0; j < original_length && j + i < size; j++) {
      if (exe_buffer[i + j] != original[j])
        break;
      if (j == original_length - 1) {
        // found a match!
        rpc_pos_t *new = malloc(sizeof(rpc_pos_t));
        new->pos = i;
        new->next = NULL;

        // attach to list
        if (!first)
          first = new;
        else
          last->next = new;
        last = new;
      }
    }
  }

  return first;
}

#define RPC_CACHE_PATH_LENGTH 1000
void rpc_init(char *prefpath) {
  char cache_file[RPC_CACHE_PATH_LENGTH + 1];
  char *exe_buffer = NULL;
  rpc_pos_t *first;
  size_t size;

  for (int i = 0; i < GTA2_PLAYER_COUNT; i++) {
    snprintf(cache_file, RPC_CACHE_PATH_LENGTH, "%sG2HR_PLAYER%i.exe", prefpath,
             i + 1);
    if (!access(cache_file, 0))
      continue;

    // read GTA2.exe, if that hasn't been done already
    if (!exe_buffer) {
      char *gta2_exe = "GTA2/gta2.exe";
      printf("loading %s...\n", gta2_exe);
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
      first = rpc_search(exe_buffer, size);
    }

    // perform search and replace
    rpc_replace(exe_buffer, size, cache_file, i, first);
  }

  if (exe_buffer) {
    free(exe_buffer);

    while (first) {
      rpc_pos_t *old = first;
      first = first->next;
      free(old);
    }
  }
}
#undef RPC_CACHE_PATH_LENGTH
