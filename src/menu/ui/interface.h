#pragma once
#include "../tk/toolkit.h"

typedef struct {
  tk_t *tk;
  tk_screen_t *main;
  tk_screen_t *credits;
  tk_screen_t *play;
  tk_screen_t *options;
} ui_t;

ui_t *ui_init(tk_t *tk);
void ui_cleanup(ui_t *ui);
