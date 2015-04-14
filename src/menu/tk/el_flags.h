#pragma once

#define TK_EL_FLAG_NONE 0
#define TK_EL_FLAG_FORCE_WIDTH 2
#define TK_EL_FLAG_FORCE_HEIGHT 4
#define TK_EL_FLAG_SELECTABLE 8
#define TK_EL_FLAG_DISABLED 16
#define TK_EL_FLAG_INVISIBLE 32

#define vk_el_width(EL, VALUE)                                                 \
  EL->width = VALUE;                                                           \
  EL->flags |= TK_EL_FLAG_FORCE_WIDTH;

#define vk_el_height(EL, VALUE)                                                \
  EL->height = VALUE;                                                          \
  EL->flags |= TK_EL_FLAG_FORCE_HEIGHT;

#define vk_el_padding(EL, LEFT, TOP, RIGHT, BOTTOM)                            \
  EL->padding_left = LEFT;                                                     \
  EL->padding_top = TOP;                                                       \
  EL->padding_right = RIGHT;                                                   \
  EL->padding_bottom = BOTTOM;

// TODO: also add inverted macros!
#define vk_el_selectable(EL) EL->flags |= TK_EL_FLAG_SELECTABLE;

#define vk_el_disabled(EL) EL->flags |= TK_EL_FLAG_DISABLED;

#define vk_el_invisible(EL) EL->flags |= TK_EL_FLAG_INVISIBLE;
