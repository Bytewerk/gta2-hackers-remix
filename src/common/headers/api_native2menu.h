#pragma once
#include "tk_actions.h"
#include <stdbool.h>
/*
        NA: native api
*/
#define G2HR_NATIVE_API_VERSION 1
#define G2HR_EXEC_AFTER_QUIT_LEN 300

//
// MENU TO NATIVE MESSAGES
//
#define NA_CLEANUP 0x01
typedef struct { char exec_after_quit[G2HR_EXEC_AFTER_QUIT_LEN]; } NA_CLEANUP_t;

#define NA_PID_TABLE 0x02
typedef struct {
  uint16_t pids[6];
  bool singleplayer;
} NA_PID_TABLE_t;

#define NA_BACK_IN_MENU 0x03

//
// NATIVE TO MENU MESSAGES
//

// perform a toolkit action
#define NA_ACTION 0x31
typedef struct {
  tk_action_t action;
  char redraw;
} NA_ACTION_t;

#define NA_CONTROLLERS_CONNECTED 0x32
typedef struct { uint16_t count; } NA_CONTROLLERS_CONNECTED_t;

#define NA_QUIT_GAME 0x33
typedef struct { char player_id; } NA_QUIT_GAME_t;
