#include "mem.h"
#include "../net/injected_net.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <stdlib.h>

mem_t *mem_init() {
  mem_t *mem = calloc(1, sizeof(mem_t));

  mem->player_id = -1;

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

    MESSAGECASESHORT(IA_ESC_TEXT_HIDE, { *GTA2_ADDR_ESC_TEXT_IS_VISIBLE = 0; });

    MESSAGECASE(IA_ESC_TEXT_SHOW, {
      *GTA2_ADDR_ESC_TEXT_IS_VISIBLE = 0;

      // FIXME: use the data to change the text etc!
      printf("FIXME, use %p!\n", data);

      *GTA2_ADDR_ESC_TEXT_IS_VISIBLE = 1;
    });
  }

  // printf("got this from native: %i\n", msg_id);
}

void mem_cleanup(mem_t *mem) { free(mem); }
