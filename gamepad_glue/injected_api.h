#pragma once

#define INJECTED_API_VERSION 2

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
#define IA_API_VERSION 0x00 // TODO
typedef struct { int version; } IA_API_VERSION_t;

#define IA_RUMBLE 0x01
typedef struct { char rumble; } IA_RUMBLE_t;

#define IA_DEBUG_TEXT 0x02
typedef struct { char text[100]; } IA_DEBUG_TEXT_t;

#define IA_VEHICLE_INFO 0x03
typedef struct { char in_vehicle; } IA_VEHICLE_INFO_t;

#define IA_SCORE_DIFF 0x04 // TODO
typedef struct { int diff; } IA_SCORE_DIFF_t;

#define IA_SCOREBOARD 0x05 // TODO
typedef struct {
  char score[6];
  int money[6];
} IA_SCOREBOARD_t;

#define IA_WINHANDLE 0x06 // TODO
typedef struct { int handle; } IA_WINHANDLE_t;

// Server to client events (without arguments)
#define IA_QUIT 0xE0 // TODO

// If you want to see hex output in the terminal,
// just use this reserved garbage byte.
#define IA_GARBAGE 0xFF