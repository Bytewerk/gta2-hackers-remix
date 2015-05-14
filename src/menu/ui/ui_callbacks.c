#include "../../common/native_api.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>

void ui_callback_for_meta(char *msg, void *userdata) {
  ui_t *ui = (ui_t *)userdata;
  int pids[5];

  if (!strcmp(msg, "CONNECTED! CAN YOU GIVE ME THE CONFIG PATH?"))
    net_send_to_meta(ui->net, ui->tk->pref_path, 0);
  else if (!strcmp(msg, "HIDE GET READY SCREEN"))
    ui->tk->screen_active = ui->ready->back;
  else if (sscanf(msg, "PID_TABLE %i %i %i %i %i %i", &pids[0], &pids[1],
                  &pids[2], &pids[3], &pids[4], &pids[5])) {
    // FIXME: use a more generic send macro here!

    char buffer[sizeof(NA_PID_TABLE_t) + 1];
    buffer[0] = NA_PID_TABLE;
    NA_PID_TABLE_t *data = (NA_PID_TABLE_t *)(buffer + 1);

    for (int i = 0; i < 6; i++)
      data->pids[i] = pids[i];

    SDLNet_TCP_Send(ui->net->sock_native, &buffer, sizeof(NA_PID_TABLE_t) + 1);
  }
}

void ui_callback_for_native(TCPsocket sock, char header, void *userdata) {
  ui_t *ui = (ui_t *)userdata;

  switch (header) {
  case NA_ACTION: {
    NA_ACTION_t data;
    SDLNet_TCP_Recv(sock, &data, sizeof(NA_ACTION_t));

    tk_action(ui->tk, data.action, SDLK_UNKNOWN);
    if (data.redraw)
      ui->tk->redraw_needed = 1;

    break;
  }
  case NA_CONTROLLERS_CONNECTED: {
    NA_CONTROLLERS_CONNECTED_t data;
    SDLNet_TCP_Recv(sock, &data, sizeof(NA_CONTROLLERS_CONNECTED_t));

    ui->controllers_connected = data.count;
    splitscreen_set_players(ui);
    break;
  }
  }
}
