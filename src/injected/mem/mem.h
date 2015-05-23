#pragma once

typedef struct {
  int player_id; // -1: unknown

  // wide chars. The addresses get found dynamically in mem.c.
  char *line1; // len: 11
  char *line2; // len: 33
  char *line3; // len: 33

} mem_t;

mem_t *mem_init();

void mem_recv_callback(unsigned char msg_id, void *userdata);

void mem_frame(mem_t *mem);

void mem_cleanup(mem_t *mem);
