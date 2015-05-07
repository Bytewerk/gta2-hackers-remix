#pragma once

// NA: native api
// WORK IN PROGRESS.

#define G2HR_NATIVE_API_VERSION 1

// MENU TO NATIVE MESSAGES
#define NA_CLEANUP 0x01

// NATIVE TO MENU MESSAGES

// send a toolkit message
#define NA_TK_ACTION 0x01
typedef struct { TK_ACTION_t action; } NA_ACTION_t;

// If you want to see hex output in the terminal,
// just use this reserved garbage byte.
#define IA_GARBAGE 0xFF
