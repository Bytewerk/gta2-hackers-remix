#include "../gta2_memory.h"
#include <stdio.h>

#define ARGS_BUFFER_SIZE 1000

void injected_thread(void *param) {
  // try to parse command line arguments from GTA2.exe
  FILE *handle = fopen("injected.log", "a");
  fputs("----- new log starts here -----\n", handle);

  fputs("hello world!\n", handle);

  // we'll use the exe name to find out which instance ID this is
  fputs("exe name: ", handle);
  fputs(EXE_NAME, handle);
  fputs("\n", handle);

  fclose(handle);
}
