#pragma once
#include "internal.h"

#define TK_BUTTON 0x00
typedef struct {
} TK_BUTTON_DATA_t;

#define TK_BOOLEAN 0x01
typedef struct { char value; } TK_BOOLEAN_DATA_t;

#define TK_INPUT 0x02
typedef struct {
  uint16_t max_length;
  char *value;
} TK_INPUT_DATA_t;

#define TK_SELECT 0x03
typedef struct {
  char selected;
  char **values;
} TK_SELECT_DATA_t;
