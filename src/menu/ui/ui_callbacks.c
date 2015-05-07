#include "ui.h"
#include <stdio.h>
#include <string.h>

void ui_callback_for_meta(char *msg, void *userdata) {
  ui_t *ui = (ui_t *)userdata;

  if (!strcmp(msg, "HIDE GET READY SCREEN")) {
    ui->tk->screen_active = ui->ready->back;
  }
}
