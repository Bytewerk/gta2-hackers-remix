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

    MESSAGECASE(IA_ESC_TEXT_SHOW, {
      if (mem->line1) {
        for (int i = 0; i < 11; i++)
          ((char *)mem->line1)[i * 2] = data->line1[i];

        for (int i = 0; i < 33; i++)
          ((char *)mem->line2)[i * 2] = data->line2[i];

        for (int i = 0; i < 33; i++)
          ((char *)mem->line3)[i * 2] = data->line3[i];
      }
    });
  }

  // printf("got this from native: %i\n", msg_id);
}

void mem_cleanup(mem_t *mem) { free(mem); }
