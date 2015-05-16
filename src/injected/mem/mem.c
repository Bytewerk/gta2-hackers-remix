#include "mem.h"
#include "../net/injected_net.h"
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
    FROMNATIVECASE(IA_PLAYER_ID, {
      mem->player_id = data->id;
      printf("got the player id: %i\n", data->id);
    })
  }
}

void mem_cleanup(mem_t *mem) { free(mem); }
