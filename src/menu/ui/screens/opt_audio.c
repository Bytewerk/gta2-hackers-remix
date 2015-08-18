#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"

typedef struct {
  ui_t *ui;
  tk_el_t *three_d;
  tk_el_t *sfx;
  tk_el_t *music;

} ud_opt_audio_t;

#define SAVE_OPT_VAL_CIRCLE(EL, OPTION_STRING)                                 \
  ini_modify(ud->ui->ini_settings, "audio", OPTION_STRING,                     \
             ((ud_circle_t *)EL->userdata)->value_str, true);
#define SAVE_OPT_VAL_BOOL(EL, OPTION_STRING)                                   \
  ini_modify(ud->ui->ini_settings, "audio", OPTION_STRING,                     \
             ((ud_arrowtext_t *)EL->userdata)->entry_selected ? "true"         \
                                                              : "false",       \
             true)

void opt_audio_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action, SDL_Keycode key) {
  ud_opt_audio_t *ud = (ud_opt_audio_t *)el->userdata;

  if (action == TK_ACTION_LEFT || action == TK_ACTION_RIGHT) {
    SAVE_OPT_VAL_CIRCLE(ud->sfx, "sfx");
    SAVE_OPT_VAL_CIRCLE(ud->music, "music");
    SAVE_OPT_VAL_BOOL(ud->three_d, "3d_sound");

    ui_apply_audio_config(ud->ui);
    ini_save(ud->ui->ini_settings, NULL, false, false);
  }
}

#undef SAVE_OPT_VAL_CIRCLE

#define GET_OPT_VAL(OPTION_STRING)                                             \
  '0' + atoi(ini_read(ui->ini_settings, "audio", OPTION_STRING))

#define SET_OPT_VAL_BOOL(EL, OPTION_STRING)                                    \
  ((ud_arrowtext_t *)EL->userdata)->entry_selected =                           \
      (strcmp(ini_read(ui->ini_settings, "audio", OPTION_STRING), "true") ==   \
       0)

tk_screen_t *ui_screen_opt_audio(tk_t *tk, ui_t *ui) {
  ud_opt_audio_t *ud = calloc(1, sizeof(ud_opt_audio_t));
  ud->ui = ui;

  tk_screen_t *ret = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(
      ret,

      TK_PARENT->bg_mashup = bg_mashup(tk->bg, NULL, "1_options", "1", NULL);

      TK_STACK(ret->el_content_container = TK_PARENT; TK_PARENT->userdata = ud;
               TK_PARENT->actionfunc = (void *)opt_audio_actionfunc;
               tk_el_padding(TK_PARENT, 300, 200, 0, 0);

               ud->three_d = tk_ctrl_arrowtext(tk, TK_PARENT, NULL, 0, NULL, 0,
                                               "3D SOUND: ", NULL, NULL,
                                               "DEFAULT: OFF", NULL, NULL);

               ud->sfx =
                   tk_ctrl_circle(tk, TK_PARENT, "SOUND EFFECT VOLUME", NULL,
                                  '0', '9', 0, 0, GET_OPT_VAL("sfx"));

               ud->music = tk_ctrl_circle(tk, TK_PARENT, "MUSIC VOLUME", NULL,
                                          '0', '9', 0, 0, GET_OPT_VAL("music"));

               SET_OPT_VAL_BOOL(ud->three_d, "3d_sound");

               ud->music->bottom_text_high = "NOTE: RADIO IS ALWAYS MUTED";
               ud->music->bottom_text_low = "IN SPLITSCREEN MODE!";););

  return ret;
}

#undef GET_OPT_VAL
#undef SET_OPT_VAL_BOOL
