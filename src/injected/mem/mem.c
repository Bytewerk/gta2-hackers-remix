#include "mem.h"
#include "../net/injected_net.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <stdlib.h>

#include <windows.h> // IsBadReadPtr, Sleep

mem_t *mem_init() {
  mem_t *mem = calloc(1, sizeof(mem_t));

  mem->player_id = -1;

  // try to find the "Is that it?" text-location
  char found = 0;
  while (!found) {
    Sleep(100);

    char *addr = ((char *)0x03bbabb2);

    for (int i = 0; i < 100; i++) {
      printf("trying addr 0x%p...", addr);

      if (!IsBadReadPtr(addr, 12)) {
        if (addr[0] == 'I' && addr[2] == 's' && addr[4] == ' ' &&
            addr[6] == 't' && addr[8] == 'h' && addr[10] == 'a' &&
            addr[12] == 't') {
          found = 1;
          break;
        }
      }
      printf("nothing!\n");
      addr += 0x00010000;
    }

    if (found) {
      addr[0] = '\0';
      mem->line1 = addr;
    }
  }
  return mem;
}

void mem_frame(mem_t *mem) {
  // if "is in vehicle" has changed, send it to the
  // native component!

  // send the score, if it has changed
}

void mem_recv_callback(unsigned char msg_id, void *userdata) {
  mem_t *mem = (mem_t *)userdata;

  switch (msg_id) {
    MESSAGECASE(IA_PLAYER_ID, {
      mem->player_id = data->id;
      printf("got the player id: %i\n", data->id);
    });

    MESSAGECASE(IA_MOVEMENT, { *GTA2_ADDR_MOVEMENT = data->movement; });

    MESSAGECASESHORT(IA_ESC_TEXT_HIDE, { mem->line1[0] = '\0'; });

    MESSAGECASESHORT(IA_ESC_TEXT_SHOW, {
      char *addr = mem->line1;
      for (int i = 0; i < 11; i++)
        addr[i * 2] = '!';
    });
  }

  // printf("got this from native: %i\n", msg_id);
}

void mem_cleanup(mem_t *mem) { free(mem); }
