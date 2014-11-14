#pragma once

#define PDLL_MOVEMENT 0xaa
#define PDLL_ERROR 0xee

typedef struct {
  char id;   // use a PDLL_* constant from above!
  short arg; // 32 bit windows: 2 bytes
} proxydll_msg_t;
