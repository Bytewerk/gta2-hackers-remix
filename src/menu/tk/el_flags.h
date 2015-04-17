#pragma once

#define TK_EL_FLAG_NONE 0
#define TK_EL_FLAG_FORCE_WIDTH 2
#define TK_EL_FLAG_FORCE_HEIGHT 4
#define TK_EL_FLAG_SELECTABLE 8
#define TK_EL_FLAG_DISABLED 16
#define TK_EL_FLAG_INVISIBLE 32

#define tk_el_width(EL, VALUE)                                                 \
  EL->width = VALUE;                                                           \
  EL->flags |= TK_EL_FLAG_FORCE_WIDTH;

#define tk_el_height(EL, VALUE)                                                \
  EL->height = VALUE;                                                          \
  EL->flags |= TK_EL_FLAG_FORCE_HEIGHT;

#define tk_el_padding(EL, LEFT, TOP, RIGHT, BOTTOM)                            \
  EL->padding_left = LEFT;                                                     \
  EL->padding_top = TOP;                                                       \
  EL->padding_right = RIGHT;                                                   \
  EL->padding_bottom = BOTTOM;

#define tk_el_selectable(EL) EL->flags |= TK_EL_FLAG_SELECTABLE

#define tk_el_disabled(EL) EL->flags |= TK_EL_FLAG_DISABLED

#define tk_el_enabled(EL) EL->flags &= ~((char)TK_EL_FLAG_DISABLED)

#define tk_el_invisible(EL) EL->flags |= TK_EL_FLAG_INVISIBLE

#define tk_el_visible(EL) EL->flags &= ~((char)TK_EL_FLAG_INVISIBLE)
