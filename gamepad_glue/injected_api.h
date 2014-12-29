#pragma once

// Client to server messages
#define IA_MOVEMENT 0x00
typedef struct { short /*2 bytes*/ movement; } IA_MOVEMENT_t;

#define IA_TEXT 0x01
typedef enum { ITEM, NETWORK, BIG } gta2_textpos_t;
typedef struct {
  gta2_textpos_t position;
  char timer;
  char text[100];
} IA_TEXT_t;

// Server to client messages
#define IA_PLAYER_COUNT 0x00
typedef struct { char count; } IA_PLAYER_COUNT_t;

#define IA_SCOREBOARD 0x01
typedef struct {
  char score[6];
  int money[6];
} IA_SCOREBOARD_t;

#define IA_WINHANDLE 0x02
typedef struct { int handle; } IA_WINHANDLE_t;

#define IA_RUMBLE 0x03
typedef struct { char rumble; } IA_RUMBLE_t;

#define IA_SCORE_DIFF 0x04
typedef struct { int diff; } IA_SCORE_DIFF_t;

#define IA_DEBUG_TEXT 0x05
typedef struct {
  char print_hex;
  char text[100];
} IA_DEBUG_TEXT_t;

// Server to client events (without arguments)
#define IA_QUIT 0xE0