#pragma once
#include <stdbool.h>
#include <strings.h>
#include <windows.h>

#define G2HR_ESC_TEXT_MAXLEN_LINE 100

typedef struct {
  HANDLE main_thread;
  int player_id;     // -1: unknown
  bool singleplayer; // only set, when the player_id != -1

  // see also: mem_init() and http://git.io/g2hr-esc-text
  // TODO: replace with text[3] below
  char *line1; // len: 11
  char *line2; // len: 33
  char *line3; // len: 33

  // 3 lines of esc-text that are always displayed
  volatile char *text[3];

  bool driving;
} mem_t;

mem_t *mem_init(HANDLE main_thread);

void mem_debug_print(char *name, char *addr, int count);

void mem_recv_callback(unsigned char msg_id, void *userdata);

void mem_prepare_esc_text(mem_t *mem);

void mem_frame(mem_t *mem);

void mem_cleanup(mem_t *mem);

void mem_zeroize(void *v, size_t n);
