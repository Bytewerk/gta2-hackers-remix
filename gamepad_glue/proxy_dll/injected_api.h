#pragma once

// Incoming messages
#define IA_IN_MOVEMENT 0x00
typedef struct { short /*2 bytes*/ movement; } IA_IN_MOVEMENT_t;

#define IA_IN_TEXT 0x01
typedef enum { ITEM, NETWORK, BIG } gta2_textpos_t;
typedef struct {
  gta2_textpos_t position;
  char timer;
  char text[100];
} IA_IN_TEXT_t;

// outgoing messages
#define IA_OUT_PLAYER_COUNT 0x00
typedef struct { char count; } IA_OUT_PLAYER_COUNT_t;

#define IA_OUT_SCOREBOARD 0x01
typedef struct {
  char score[6];
  int money[6];
} IA_OUT_SCOREBOARD_t;

#define IA_OUT_WINHANDLE 0x02
typedef struct { int handle; } IA_OUT_WINHANDLE_t;

#define IA_OUT_RUMBLE 0x03
typedef struct { char byte; } IA_OUT_RUMBLE_t;

#define IA_OUT_SCORE_DIFF 0x04
typedef struct { int diff; } IA_OUT_SCORE_DIFF_t;

// outgoing events (without arguments)
#define IA_OUT_QUIT 0xE0
