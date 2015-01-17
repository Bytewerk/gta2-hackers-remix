#pragma once
#include "../toolkit/toolkit.h"
#include "ui_text.h"

typedef struct {
  tk_screen_t *main;
  tk_screen_t *credits;

} ui_t;

ui_t *ui_init(tk_t *tk);
void ui_cleanup(tk_t *tk, ui_t *ui);
