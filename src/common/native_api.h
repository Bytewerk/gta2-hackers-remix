#pragma once
#include "tk_actions.h"

// NA: native api
// WORK IN PROGRESS.

#define G2HR_NATIVE_API_VERSION 1

// MENU TO NATIVE MESSAGES
#define NA_CLEANUP 0x01

// NATIVE TO MENU MESSAGES

// send a toolkit message
#define NA_TK_ACTION 0x01
typedef struct { tk_action_t action; } NA_ACTION_t;
