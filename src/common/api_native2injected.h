#pragma once
#include <inttypes.h>

// Native to injected messages
#define IA_MOVEMENT 0xa0
typedef struct { short /*2 bytes*/ movement; } IA_MOVEMENT_t;

#define IA_TEXT 0xa1
typedef enum { ITEM, NETWORK, BIG } gta2_textpos_t;
typedef struct {
  gta2_textpos_t position;
  char timer;
  char text[100];
} IA_TEXT_t;

#define IA_PLAYER_ID 0xa2
typedef struct { char id; } IA_PLAYER_ID_t;

// Injected to native messages

#define IA_PID 0x01
typedef struct { uint16_t pid; } IA_PID_t;

#define IA_RUMBLE 0x02
typedef struct { char rumble; } IA_RUMBLE_t;

#define IA_VEHICLE_INFO 0x03
typedef struct { char in_vehicle; } IA_VEHICLE_INFO_t;

#define IA_SCORE_DELTA 0x04 // TODO
typedef struct { int delta; } IA_SCORE_DELTA_t;

#define IA_SCOREBOARD 0x05 // TODO
typedef struct {
  int score[6];
  int money[6];
} IA_SCOREBOARD_t;

// Server to client events (without arguments)
#define IA_QUIT 0xE0 // TODO

// If you want to see hex output in the terminal,
// just use this reserved garbage byte.
#define IA_GARBAGE 0xFF
