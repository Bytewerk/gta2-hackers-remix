#include "mem.h"
#include "../net/injected_net.h"
#include "mem_gta2_addresses.h"
#include <stdio.h>
#include <stdlib.h>

void mem_frame(mem_t *mem) {
  // if "is in vehicle" has changed, send it to the
  // native component!

  // send the score, if it has changed
}

void mem_recv_callback(unsigned char msg_id, void *userdata) {
  mem_t *mem = (mem_t *)userdata;

  switch (msg_id) {
    MESSAGECASE(IA_PLAYER_ID, { mem->player_id = data->id; });

    MESSAGECASE(IA_MOVEMENT, { *GTA2_ADDR_MOVEMENT = data->movement; });

    MESSAGECASESHORT(IA_ESC_TEXT_HIDE, { mem_text_clear(mem); });

    MESSAGECASESHORT(IA_ESC_TEXT_SHOW, {
      if (mem->line1) {
        mem->line1[0] = 'I';
        mem->line2[0] = 'P';
        mem->line3[0] = 'P';
      }
    });
  }

  // printf("got this from native: %i\n", msg_id);
}

void mem_cleanup(mem_t *mem) { free(mem); }
