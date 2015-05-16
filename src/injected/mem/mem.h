#pragma once

typedef struct {
  int player_id; // -1: unknown

} mem_t;

mem_t *mem_init();

void mem_recv_callback(unsigned char msg_id, void *userdata);

void mem_frame(mem_t *mem);

void mem_cleanup(mem_t *mem);
