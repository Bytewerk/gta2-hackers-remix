#include "../../common/native_api.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>

void ui_callback_for_meta(char *msg, void *userdata) {
  ui_t *ui = (ui_t *)userdata;

  if (!strcmp(msg, "HIDE GET READY SCREEN")) {
    ui->tk->screen_active = ui->ready->back;
  }
}

void ui_callback_for_native(TCPsocket sock, char header, void *userdata) {
  ui_t *ui = (ui_t *)userdata;

  printf("[menu] debug: parsing callback from native, header: %i\n", header);

  switch (header) {
  case NA_ACTION: {
    NA_ACTION_t data;
    SDLNet_TCP_Recv(sock, &data, sizeof(NA_ACTION_t));

    tk_action_exec(ui->tk, data.action, SDLK_UNKNOWN);
    if (data.redraw)
      ui->tk->redraw_needed = 1;

    break;
  }
  case NA_CONTROLLERS_CONNECTED: {
    NA_CONTROLLERS_CONNECTED_t data;
    SDLNet_TCP_Recv(sock, &data, sizeof(NA_CONTROLLERS_CONNECTED_t));

    ui->controllers_connected = data.count;

    break;
  }
  }
}
