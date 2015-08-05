#include "mem.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h> // GetModuleHandle

mem_t *mem_init() {
  // Get the base offset of GTA2.exe
  // see also: http://stackoverflow.com/a/17965759
  EXE_OFFSET = (char *)GetModuleHandle(0);

  mem_t *mem = calloc(1, sizeof(mem_t));
  mem->player_id = -1;

  mem_prepare_esc_text(mem);

  return mem;
}
