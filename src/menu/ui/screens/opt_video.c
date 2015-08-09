#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include <stdbool.h>

// USERDATA STRUCT
typedef struct {
  ui_t *ui;
  tk_el_t *fullscreen;
  tk_el_t *upscaling;
  tk_el_t *lighting;
  tk_el_t *gamma;
  tk_el_t *exploding_scores;

} ud_opt_video_t;

#define SAVE_OPT_VAL(EL, OPTION_STRING)                                        \
  {                                                                            \
    uint16_t val = ((ud_arrowtext_t *)EL->userdata)->entry_selected;           \
    char *str = ((ud_arrowtext_t *)EL->userdata)->entries[val];                \
    ini_modify(ud->ui->ini_settings, "video", OPTION_STRING, str, true);       \
  }

#define SAVE_OPT_VAL_BOOL(EL, OPTION_STRING)                                   \
  ini_modify(ud->ui->ini_settings, "video", OPTION_STRING,                     \
             ((ud_arrowtext_t *)EL->userdata)->entry_selected ? "true"         \
                                                              : "false",       \
             true)

void opt_video_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key) {
  ud_opt_video_t *ud = (ud_opt_video_t *)el->userdata;

  if (action == TK_ACTION_LEFT || action == TK_ACTION_RIGHT ||
      action == TK_ACTION_ENTER) {
    SAVE_OPT_VAL_BOOL(ud->fullscreen, "fullscreen");
    SAVE_OPT_VAL(ud->upscaling, "menu_upscaling");
    SAVE_OPT_VAL(ud->lighting, "ingame_lighting");
    SAVE_OPT_VAL(ud->gamma, "gamma");
    SAVE_OPT_VAL_BOOL(ud->exploding_scores, "exploding_scores");

    ui_apply_video_config(ud->ui);
    ini_save(ud->ui->ini_settings, NULL, false, false);
  }
}
#undef SAVE_OPT_VAL
#undef SAVE_OPT_VAL_BOOL

#define SET_OPT_VAL_BOOL(EL, OPTION_STRING)                                    \
  ((ud_arrowtext_t *)EL->userdata)->entry_selected =                           \
      (strcmp(ini_read(ui->ini_settings, "video", OPTION_STRING), "true") ==   \
       0)
#define SET_OPT_VAL(EL, OPTION_STRING)                                         \
  tk_ctrl_arrowtext_set_value_by_string(                                       \
      EL, ini_read(ui->ini_settings, "video", OPTION_STRING))

tk_screen_t *ui_screen_opt_video(tk_t *tk, ui_t *ui) {
  ud_opt_video_t *ud = calloc(1, sizeof(ud_opt_video_t));
  ud->ui = ui;

  tk_screen_t *ret = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(
      ret,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, NULL, "1_options", "1", NULL);

      TK_STACK(ret->el_content_container = TK_PARENT; TK_PARENT->userdata = ud;
               TK_PARENT->actionfunc = (void *)opt_video_actionfunc;
               tk_el_padding(TK_PARENT, 300, 250, 0, 0);

               // create controls
               ud->fullscreen = tk_ctrl_arrowtext(
                   tk, TK_PARENT, NULL, 0, NULL, 0, "FULLSCREEN: ", NULL,
                   tk->wine ? "RESTART REQUIRED TO APPLY" : NULL,
                   "SEE ALSO: GIT.IO/G2HR_MENU", NULL, NULL);

               ud->upscaling = tk_ctrl_arrowtext(
                   tk, TK_PARENT, NULL, 0, ui->menu_upscaling_values->pieces,
                   ui->menu_upscaling_values->count, "UPSCALING: ", NULL,
                   "ONLY FOR THE MENU", "DEFAULT: LINEAR", NULL, NULL);

               ud->lighting = tk_ctrl_arrowtext(
                   tk, TK_PARENT, NULL, 0, ui->ingame_lighting_values->pieces,
                   ui->ingame_lighting_values->count, "LIGHTING: ", NULL, NULL,
                   "DEFAULT: NOON", NULL, NULL);

               ud->gamma = tk_ctrl_arrowtext(
                   tk, TK_PARENT, NULL, 0, ui->gamma_values->pieces,
                   ui->gamma_values->count, "GAMMA: ", NULL, NULL,
                   "DEFAULT: 10", NULL, NULL);

               ud->exploding_scores = tk_ctrl_arrowtext(
                   tk, TK_PARENT, NULL, 0, NULL, 0, "SCORE EFFECTS: ", NULL,
                   "INGAME EXPLODING SCORES", tk->wine ? "DEFAULT: ON" : NULL,
                   NULL, NULL);

               // set values from config
               SET_OPT_VAL_BOOL(ud->fullscreen, "fullscreen");
               SET_OPT_VAL(ud->upscaling, "menu_upscaling");
               SET_OPT_VAL(ud->lighting, "ingame_lighting");
               SET_OPT_VAL(ud->gamma, "gamma");
               SET_OPT_VAL_BOOL(ud->exploding_scores, "exploding_scores");));

  return ret;
}
#undef SET_OPT_VAL
#undef SET_OPT_VAL_BOOL
