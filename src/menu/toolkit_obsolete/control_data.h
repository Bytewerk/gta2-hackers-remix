#pragma once
#include "internal.h"

// Editing player name:
// TK_LIST, onclick -> change to textfield - or something like that

// onclick func gets executed before onclick_screen
// both may be set to NULL
#define TK_BUTTON 0x00
typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
} TK_BUTTON_DATA_t;

#define TK_BOOLEAN 0x01
typedef struct { char value; } TK_BOOLEAN_DATA_t;

// usually just a line, maybe add a multiline-boolean later?
#define TK_TEXTFIELD 0x02
typedef struct {
  uint16_t max_length;
  char *value;
} TK_TEXTFIELD_DATA_t;

#define TK_LIST 0x03
typedef struct {
  char value;
  char **entries;
} TK_LIST_DATA_t;

// min, max are really chars, eg: '0' - 'B'
#define TK_CIRCLE 0x04
typedef struct {
  char value;
  char min;
  char max;
} TK_CIRCLE_DATA_t;

// something like that. draw_func should get x,y as coordinates.
// not really imeplemented yet!
// this could be used to draw split screen layouts.
#define TK_IMAGE 0x05
typedef struct {
  uint16_t width;
  uint16_t height;
  void *draw_func;
} TK_IMAGE_DATA_t;
