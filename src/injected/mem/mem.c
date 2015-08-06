#include "mem.h"
#include "../net/injected_net.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // IsBadReadPtr, Sleep

/*
        copied from: mbedtls_zeroize
        Implementation that should never be optimized out by the compiler
*/
void mem_zeroize(void *v, size_t n) {
  volatile unsigned char *p = v;
  while (n--)
    *p++ = 0;
}

void mem_debug_print(char *name, char *addr, int count) {
  printf("%s at %p:\n", name, addr);
  for (int i = 0; i < count; i++) {
    if (IsBadReadPtr(addr + i, 1)) {
      printf("%p: Bad pointer, stopping here.\n", addr + i);
      return;
    }

    printf("\t%s+%03i: %02x %c\n", name, i, (unsigned char)addr[i], addr[i]);
  }
}

void mem_frame(mem_t *mem) {
  // send a message when the player has "borrowed" a vehicle, or
  // left it
  if (*GTA2_ADDR_PLAYER_IN_VEHICLE != mem->driving)
    MESSAGESEND(IA_VEHICLE_INFO, data->in_vehicle = mem->driving =
                                     *GTA2_ADDR_PLAYER_IN_VEHICLE;);

  // TODO: send the score, if it has changed
}

void mem_recv_callback(unsigned char msg_id, void *userdata) {
  mem_t *mem = (mem_t *)userdata;

  switch (msg_id) {
    MESSAGECASE(IA_PLAYER_ID, {
      mem->player_id = data->id;
      mem->singleplayer = data->singleplayer;
    });

    MESSAGECASE(IA_MOVEMENT, { *GTA2_ADDR_MOVEMENT = data->movement; });

    MESSAGECASE(IA_ESC_TEXT_SHOW, {
      if (mem->text[0]) {
        // FIXME: we can use far more characters than 11 and 33,
        // but this needs to be specified in the
        // injected<>native protocol first.

        for (int i = 0; i < 11; i++)
          mem->text[0][i * 2] = data->line1[i];

        for (int i = 0; i < 33; i++)
          mem->text[1][i * 2] = data->line2[i];

        for (int i = 0; i < 33; i++)
          mem->text[2][i * 2] = data->line3[i];
      }
    });
  }

  // printf("got this from native: %i\n", msg_id);
}

void mem_cleanup(mem_t *mem) { free(mem); }
