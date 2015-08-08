#include "ui.h"

// load the ini file, set default values and split strings where
// necessary

// user config: gets modified by user only
// ui config: gets modified by the ui (saved on exit!)

void ui_init_configs(ui_t *ui) {
  ini_t *user = ini_open("data/g2hr.ini", true, false);

  ini_modify(user, "slotmachine", "enabled", "false", false);
  ini_modify(user, "slotmachine", "cmd_shutdown", "poweroff", false);
  ini_modify(user, "slotmachine", "cmd_shutdown", "reboot", false);
  ini_modify(user, "multiplayer", "time", "3 5 10 15 20 25 30", false);

  ini_modify(user, "video", "window_width", "640", false);
  ini_modify(user, "video", "window_height", "480", false);
  ini_modify(user, "video", "scale_quality", "linear", false);

  ini_t *settings =
      ini_open(cstr_merge(ui->tk->pref_path, "ui.ini"), false, false);
  ini_modify(settings, "ui", "update_check_enabled", "ask", false);
  ini_modify(settings, "video", "fullscreen", "true", false);

  ui->multiplayer_time_values =
      cstr_split(ini_read(user, "multiplayer", "time"), ' ', false);

  ui->ini_usersettings = user;
  ui->ini_settings = settings;
}

void ui_cleanup_configs(ui_t *ui) {
  ini_cleanup(ui->ini_usersettings);
  ini_save(ui->ini_settings, NULL, false, false);

  ini_cleanup(ui->ini_settings);

  cstr_split_free(ui->multiplayer_time_values);
}
