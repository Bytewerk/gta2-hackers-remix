#pragma once

#define ACTION_LIMIT_RATE 100 // miliseconds

typedef enum {
  TK_ACTION_NONE,
  TK_ACTION_UP,
  TK_ACTION_DOWN,
  TK_ACTION_LEFT,
  TK_ACTION_RIGHT,
  TK_ACTION_ENTER,
  TK_ACTION_BACK,

  TK_ACTION_MOUSEMOVE,
  TK_ACTION_MOUSEDOWN,

  // this isn't a real action. All actions before this limiter
  // in this list get rate limited!
  TK_ACTION_LAST_LIMITED,

  TK_ACTION_TYPING,
  TK_ACTION_REDRAW,

} tk_action_t;
