#include "rpc.h"
#include "../../common/fs/fs.h"
#include "../../common/headers/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RPC_SEARCH_STR "\\DMA Design Ltd\\GTA2\\"
void rpc_replace(char *exe_buffer, uint32_t size, char *cache_file,
                 int player_num, rpc_pos_t *pos) {
  printf("writing %s...\n", cache_file);

  // generate the replaced-string
  char *original = RPC_SEARCH_STR;
  int original_len = strlen(original);
  char replaced[original_len + 1];
  sprintf(replaced, "\\GTA2HackersRemix\\P%i\\", player_num + 1);

  FILE *handle = fopen(cache_file, "wb");

  uint32_t pos_last = 0;
  while (pos) {
    uint32_t pos_end = pos->pos;

    fwrite(exe_buffer + pos_last, pos_end - pos_last, 1, handle);
    fwrite(replaced, original_len, 1, handle);

    pos_last = pos->pos + original_len;
    pos = pos->next;
  }

  // write the rest, close the file
  fwrite(exe_buffer + pos_last, size - pos_last, 1, handle);
  fclose(handle);
}

/*
        When pressing ESC in the game, it shows 3 lines of text
        (quit1...quit3), "Is that it?" etc.
        In order to draw text on the game screen, we force this to be always
        on and then change the messages in memory.

        This works by replacing a conditional jump with NOP (no operation)
        bytes in the binary code.
*/
void rpc_apply_always_show_esc_dialog_hack(char *exe_buffer, uint32_t size) {
  // This is for vike's exe.
  // Address in the GTA2 freeware exe is: 0xC8725
  char *pos = exe_buffer + 0xD8725;
  for (int i = 0; i < 6; i++)
    pos[i] = 0x90; // NOP
}

rpc_pos_t *rpc_search(char *exe_buffer, uint32_t size) {
  rpc_pos_t *first = NULL;
  rpc_pos_t *last;

  char *original = RPC_SEARCH_STR;
  int original_length = strlen(original);

  for (uint32_t i = 0; i < size; i++) {
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
  uint32_t size;

  for (int i = 0; i < GTA2_PLAYER_COUNT; i++) {
    snprintf(cache_file, RPC_CACHE_PATH_LENGTH, "%sG2HR_PLAYER%i.exe", prefpath,
             i + 1);
    if (!access(cache_file, 0))
      continue;

    // read GTA2.exe, if that hasn't been done already
    if (!exe_buffer) {
      exe_buffer = fs_load_small_file_to_ram("GTA2/gta2.exe", &size, 0);

      // find the registry strings we'll replace
      first = rpc_search(exe_buffer, size);

      // apply additional hacks
      rpc_apply_always_show_esc_dialog_hack(exe_buffer, size);
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
