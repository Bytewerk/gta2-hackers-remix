#include "ui.h"
#include <stdlib.h>

#define BUFFER_LEN 200
#define RANDBOOL (rand() > RAND_MAX / 2)

// TODO: add parameter: is splitscreen game and then always mute music

void ui_send_game_config(ui_t *ui, bool splitscreen) {
  char buffer[BUFFER_LEN + 1];

  // music volume
  snprintf(buffer, BUFFER_LEN, "CONFIG MUSIC_VOL %s",
           splitscreen ? "0" : ini_read(ui->ini_settings, "audio", "music"));
  net_send_to_meta(ui->net, buffer, 0);

  // sfx volume
  snprintf(buffer, BUFFER_LEN, "CONFIG SFX_VOL %s",
           ini_read(ui->ini_settings, "audio", "sfx"));
  net_send_to_meta(ui->net, buffer, 0);

  // lighting
  char *lighting = ini_read(ui->ini_settings, "video", "ingame_lighting");
  if (!strcmp(lighting, "random"))
    lighting = RANDBOOL ? "dusk" : "noon";
  snprintf(buffer, BUFFER_LEN, "CONFIG LIGHTING %s", lighting);
  net_send_to_meta(ui->net, buffer, 0);

  // gamma
  snprintf(buffer, BUFFER_LEN, "CONFIG GAMMA %s",
           ini_read(ui->ini_settings, "video", "gamma"));
  net_send_to_meta(ui->net, buffer, 0);

  // exploding scores
  snprintf(buffer, BUFFER_LEN, "CONFIG EXPLODING_SCORES %s",
           ini_read(ui->ini_settings, "video", "exploding_scores"));
  net_send_to_meta(ui->net, buffer, 0);
}
