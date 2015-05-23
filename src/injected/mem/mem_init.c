#include "mem.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h> // IsBadReadPtr, Sleep

// Find the "Is that it?" text-location with an educated guess.
// documentation: http://git.io/g2hr-esc-text
void mem_init_find_is_that_it(mem_t *mem) {
  Sleep(1000);

  int tries = 0;
  while (!mem->line1 && tries < 100) {
    for (int i = 0; i < 10000 && !mem->line1; i++) {
      char *addr = GTA2_ADDR_ESC_TEXT_ALIGNMENT + i * 0x00010000;

      if (!IsBadReadPtr(addr, 12) &&
          !wcscmp((wchar_t *)addr, GTA2_ESC_TEXT_QUIT1)) {
        mem->line1 = addr;
      }
    }
    tries++;

    if (!mem->line1) {
      printf("quit string not found, try #%i...\n", tries);
      Sleep(100);
    }
  }

  if (mem->line1) {
    mem->line2 = (char *)mem->line1 + 0x18;
    mem->line3 = (char *)mem->line1 + 0x5c;

    // hide the text initially
    mem->line1[0] = '\0';
    mem->line2[0] = '\0';
    mem->line3[0] = '\0';
  } else
    printf("Failed to find the 'Is that it'-text!\n");
}

mem_t *mem_init() {
  mem_t *mem = calloc(1, sizeof(mem_t));
  mem->player_id = -1;

  mem_init_find_is_that_it(mem);
  return mem;
}
