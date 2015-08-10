#include "mem.h"
#include "mem_gta2_addresses.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h> // IsBadReadPtr, Sleep

// documentation: http://git.io/g2hr-esc-text

bool mem_wait_for_string_table() {
  for (int i = 0; i < 100; i++) {
    Sleep(100);
    if (IsBadReadPtr(GTA2_ADDR_STRING_TABLE_BASE, 10))
      continue;

    char *table = GTA2_ADDR_STRING_TABLE;
    if (IsBadReadPtr(table, 10))
      continue;

    char *offset = GTA2_ADDR_STRING_OFFSET;
    if (IsBadReadPtr(offset, 20))
      continue;
    if (offset[0] == 'y' && offset[1] == '!' && offset[3] == 0x00 &&
        offset[5] == 0x00 && offset[7] == 0x00)
      return true;
  }
  printf("ERROR: couldn't find the string table!\n");
  return false;
}

void mem_prepare_esc_text(mem_t *mem) {
  if (!mem_wait_for_string_table())
    return;

  for (int i = 0; i < 3; i++) {
    mem->text[i] = malloc(G2HR_ESC_TEXT_MAXLEN_LINE);
    mem_zeroize((char *)mem->text[i], G2HR_ESC_TEXT_MAXLEN_LINE);
  }

  for (int tries = 0; tries < 5; tries++) {
    for (char *pos = GTA2_ADDR_STRING_TABLE; pos < GTA2_ADDR_STRING_OFFSET;
         pos += 12) {
      char *name = pos + 4;
      if (IsBadReadPtr(name, 8))
        break;

      if (strncmp(name, "quit1", 8) == 0 || strncmp(name, "quit2", 8) == 0 ||
          strncmp(name, "quit3", 8) == 0) {
        int num = (name[4] - '1');
        SuspendThread(mem->main_thread);
        *(volatile char **)pos = mem->text[num];
        ResumeThread(mem->main_thread);

        if (num == 2)
          return;
      }
    }
  }
  printf("ERROR: failed to inject the custom esc-text!\n");
}
