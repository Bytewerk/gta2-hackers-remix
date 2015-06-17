#pragma once
#include <inttypes.h>
#include <stdbool.h>

// Native to injected messages
#define IA_MOVEMENT 0xa0
typedef struct { uint16_t movement; } IA_MOVEMENT_t;

#define IA_PLAYER_ID 0xa1
typedef struct {
  int id;
  bool singleplayer;
} IA_PLAYER_ID_t;

#define IA_ESC_TEXT_HIDE 0xa2
#define IA_ESC_TEXT_SHOW 0xa3
typedef struct {
  char line1[11];
  char line2[33];
  char line3[33];
  uint16_t timeout_ms; // (not implemented yet, set to 0)
} IA_ESC_TEXT_SHOW_t;

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

#define IA_INIT_COMPLETE 0x06
