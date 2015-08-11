#include "../../common/headers/api_native2menu.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>

void ui_callback_for_meta(char *msg, void *userdata) {
  ui_t *ui = (ui_t *)userdata;
  int pids[6];
  char buffer[200];

  if (!strcmp(msg, "CONNECTED! CAN YOU GIVE ME THE CONFIG PATH?"))
    net_send_to_meta(ui->net, ui->tk->pref_path, 0);
  else if (!strcmp(msg, "SHUT UP, THE GAME HAS BEEN STARTED!")) {
    // hide the 'get ready...' screen
    ui->tk->screen_active = ui->ready->back;
    ui->tk->disable_draw = true;
    SDL_RenderClear(ui->tk->renderer);
    SDL_RenderPresent(ui->tk->renderer);
    Mix_FadeOutMusic(G2HR_MUSIC_FADE_OUT_TIME_MS);
  } else if (!strcmp(msg, "INSTANCE CLOSED 0")) {
    sfx_play_song(ui->tk->sfx, SFX_SONG_MAINMENU);
    ui->tk->disable_draw = false;
    MESSAGESENDSHORT(ui->net->sock_native, NA_BACK_IN_MENU);
  }

  else if (sscanf(msg, "PID_TABLE %i %i %i %i %i %i", &pids[0], &pids[1],
                  &pids[2], &pids[3], &pids[4], &pids[5])) {
    MESSAGESEND(ui->net->sock_native, NA_PID_TABLE,

                for (int i = 0; i < 6; i++) data->pids[i] = pids[i];

                data->singleplayer = ui->singleplayer;);
  } else if (sscanf(msg, "STATUS %s", buffer)) {
    ui_ready_status(ui, msg + strlen("STATUS "));
  }
}

void ui_callback_for_native(TCPsocket sock, char header, void *userdata) {
  ui_t *ui = (ui_t *)userdata;

  // TODO: use message macros!
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

    MESSAGECASE(sock, NA_QUIT_GAME, {
      char *buffer = malloc(20);
      snprintf(buffer, 100, "QUIT %i", data->player_id);
      net_send_to_meta(ui->net, buffer, 1);
    });
  }
}
