#include "ui.h"
#include <stdlib.h>

#define BUFFER_LEN 200
#define RANDBOOL (rand() > RAND_MAX / 2)

void ui_send_game_config(ui_t *ui, bool splitscreen) {
  char buffer[BUFFER_LEN + 1];

  // 3d sound
  snprintf(buffer, BUFFER_LEN, "CONFIG 3D_SOUND %s",
           ini_read(ui->ini_settings, "audio", "3d_sound"));
  net_send_to_meta(ui->net, buffer, 0);

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

  // exploding scores
  snprintf(buffer, BUFFER_LEN, "CONFIG EXPLODING_SCORES %s",
           ini_read(ui->ini_settings, "video", "exploding_scores"));
  net_send_to_meta(ui->net, buffer, 0);

  // blood
  snprintf(buffer, BUFFER_LEN, "CONFIG BLOOD %s",
           ini_read(ui->ini_settings, "video", "blood"));
  net_send_to_meta(ui->net, buffer, 0);

  // show player names
  snprintf(buffer, BUFFER_LEN, "CONFIG SHOW_NAMES %s",
           ini_read(ui->ini_settings, "video", "show_names"));
  net_send_to_meta(ui->net, buffer, 0);

  // text speed
  snprintf(buffer, BUFFER_LEN, "CONFIG TEXT_SPEED %s",
           ini_read(ui->ini_settings, "video", "text_speed"));
  net_send_to_meta(ui->net, buffer, 0);
}
