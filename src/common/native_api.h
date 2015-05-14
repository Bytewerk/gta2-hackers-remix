#pragma once
#include "tk_actions.h"
/*
        NA: native api
*/
#define G2HR_NATIVE_API_VERSION 1

//
// MENU TO NATIVE MESSAGES
// they all have one byte length
//
#define NA_CLEANUP 0x01

#define NA_PID_TABLE 0x02
typedef struct { uint16_t pids[6]; } NA_PID_TABLE_t;

// only possible, when in slotmachine mode
#define NA_POWEROFF 0x20
#define NA_REBOOT 0x21

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
