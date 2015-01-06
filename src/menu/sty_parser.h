#pragma once
#include <stdint.h>

typedef struct {
  uint16_t font_count;
  uint16_t *base;
} font_base_t;

typedef struct { font_base_t *font_base; } sty_t;

sty_t *sty_parser(char *filename);
void sty_cleanup(sty_t *sty);
