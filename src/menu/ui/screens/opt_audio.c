#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"

/*
        TODO:
                - add bottom text for ingame radio volume, that states that
                        multiplayer radio is always muted
                - add screen userdata
                - save the values in the userdata
                - use the values to actually set the volumes
                - save and load the values to/from the settings ini-file
*/

tk_screen_t *ui_screen_opt_audio(tk_t *tk, ui_t *ui) {
  tk_screen_t *ret = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(ret,

                  TK_PARENT->bg_mashup =
                      bg_mashup(tk->bg, NULL, "1_options", "1", NULL);

                  tk_el_padding(TK_PARENT, 300, 150, 0, 0);

                  tk_ctrl_circle(tk, TK_PARENT, "MENU SFX VOLUME", NULL, '0',
                                 '9', 0, 0, '9');

                  tk_ctrl_circle(tk, TK_PARENT, "MENU MUSIC VOLUME", NULL, '0',
                                 '9', 0, 0, '9');

                  tk_ctrl_circle(tk, TK_PARENT, "INGAME SFX VOLUME", NULL, '0',
                                 '9', 0, 0, '9');

                  tk_ctrl_circle(tk, TK_PARENT, "INGAME RADIO VOLUME", NULL,
                                 '0', '9', 0, 0, '0');

                  );

  return ret;
}
