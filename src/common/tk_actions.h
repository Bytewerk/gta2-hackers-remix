#pragma once

#define G2HR_TK_ACTIONS_VERSION 1
#define ACTION_LIMIT_RATE 100 // milliseconds

typedef enum {
  TK_ACTION_NONE,
  TK_ACTION_UP,
  TK_ACTION_DOWN,
  TK_ACTION_LEFT,
  TK_ACTION_RIGHT,
  TK_ACTION_ENTER,
  TK_ACTION_ESC,
  TK_ACTION_BACKSPACE,

  TK_ACTION_MOUSEMOVE,
  TK_ACTION_MOUSEDOWN,

  TK_ACTION_DEBUGDRAW,

  // this isn't a real action. All actions before this limiter
  // in this list get rate limited!
  TK_ACTION_LAST_LIMITED,

  TK_ACTION_BEFORE_FIRST_SCREEN_FRAME,
  TK_ACTION_TYPING,
  TK_ACTION_REDRAW, // window has been resized
  TK_ACTION_DRAW_BEFORE_CONTROLS,
  TK_ACTION_FRAMETIME,
  TK_ACTION_CLEANUP,

} tk_action_t;
