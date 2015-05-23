#pragma once
#include <wchar.h>

typedef struct {
  int player_id; // -1: unknown

  // see also: mem_init() and http://git.io/g2hr-esc-text
  wchar_t *line1; // len: 11
  wchar_t *line2; // len: 33
  wchar_t *line3; // len: 33

} mem_t;

#define mem_text_clear(MEM)                                                    \
  if (mem->line1)                                                              \
  mem->line1[0] = mem->line2[0] = mem->line3[0] = '\0'

mem_t *mem_init();

void mem_recv_callback(unsigned char msg_id, void *userdata);

void mem_frame(mem_t *mem);

void mem_cleanup(mem_t *mem);
