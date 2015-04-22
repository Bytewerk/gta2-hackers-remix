#pragma once

/*
        H_CENTER:
                This only works, when the parent element exists and has a forced
                width!
*/

#define TK_EL_FLAG_NONE 0
#define TK_EL_FLAG_FORCE_WIDTH 2
#define TK_EL_FLAG_FORCE_HEIGHT 4
#define TK_EL_FLAG_SELECTABLE 8
#define TK_EL_FLAG_DISABLED 16
#define TK_EL_FLAG_INVISIBLE 32
#define TK_EL_FLAG_H_CENTER 64
#define TK_EL_FLAG_SCROLLABLE 128
#define TK_EL_FLAG_FREE_TEXT 256

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

// change flags:
#define tk_el_selectable(EL) EL->flags |= TK_EL_FLAG_SELECTABLE
#define tk_el_disabled(EL) EL->flags |= TK_EL_FLAG_DISABLED
#define tk_el_invisible(EL) EL->flags |= TK_EL_FLAG_INVISIBLE
#define tk_el_center(EL) EL->flags |= TK_EL_FLAG_H_CENTER
#define tk_el_scrollable(EL) EL->flags |= TK_EL_FLAG_SCROLLABLE
#define tk_el_free_text(EL) EL->flags |= TK_EL_FLAG_FREE_TEXT

// reset to default:
#define tk_el_unselectable(EL) EL->flags &= ~((uint16_t)TK_EL_FLAG_SELECTABLE)
#define tk_el_enabled(EL) EL->flags &= ~((uint16_t)TK_EL_FLAG_DISABLED)
#define tk_el_visible(EL) EL->flags &= ~((uint16_t)TK_EL_FLAG_INVISIBLE)
#define tk_el_left_aligned(EL) EL->flags &= ~((uint16_t)TK_EL_FLAG_H_CENTER)
#define tk_el_fixed(EL) EL->flags &= ~((uint16_t)TK_EL_FLAG_SCROLLABLE)
