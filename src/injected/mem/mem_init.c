#include "mem.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h> // IsBadReadPtr, Sleep

// Find the "Is that it?" text-location with an educated guess.
// documentation: http://git.io/g2hr-esc-text
// TODO: fully rewrite that part!

mem_t *mem_init() {
  // Get the base offset of GTA2.exe
  // see also: http://stackoverflow.com/a/17965759
  EXE_OFFSET = (char *)GetModuleHandle(0);

  mem_t *mem = calloc(1, sizeof(mem_t));
  mem->player_id = -1;

  printf("Sleeping for 10s, so hopefully the string table is loaded...\n");
  for (int i = 0; i < 10; i++) {
    Sleep(1000);
    printf("%i ", i);
  }
  printf("\n");

  mem_debug_print("GTA2_ADDR_STRING_TABLE", GTA2_ADDR_STRING_TABLE, 100);
  mem_debug_print("GTA2_ADDR_STRING_OFFSET", GTA2_ADDR_STRING_OFFSET, 100);

  return mem;
}
