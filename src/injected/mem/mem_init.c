#include "mem.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <windows.h> // IsBadReadPtr, Sleep

// Find the "Is that it?" text-location with an educated guess.
// documentation: http://git.io/g2hr-esc-text
void mem_init_find_is_that_it(mem_t *mem) {
  int tries = 0;
  while (!mem->line1 && tries < 100) {
    Sleep(100);
    wchar_t *addr = GTA2_ADDR_ESC_TEXT_ALIGNMENT;

    for (int i = 0; i < 100; i++) {
      if (!IsBadReadPtr(addr, 12) && !wcscmp(addr, GTA2_ESC_TEXT_QUIT1))
        mem->line1 = addr;
      addr += 0x00010000;
    }
    tries++;
  }

  if (mem->line1) {
    mem->line2 = (wchar_t *)((char *)mem->line1 + 0x18);
    mem->line3 = (wchar_t *)((char *)mem->line1 + 0x5c);
    mem_text_clear(mem);
  } else
    printf("Failed to find the 'Is that it'-text!\n");
}

mem_t *mem_init() {
  mem_t *mem = calloc(1, sizeof(mem_t));
  mem->player_id = -1;

  mem_init_find_is_that_it(mem);
  return mem;
}
