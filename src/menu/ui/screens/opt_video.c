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

tk_screen_t *ui_screen_opt_video(tk_t *tk, ui_t *ui) {
  tk_screen_t *ret = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(ret,

                  TK_PARENT->bg_mashup =
                      bg_mashup(tk->bg, NULL, "1_options", "1", NULL);

                  tk_el_padding(TK_PARENT, 300, 150, 0, 0);

                  tk_ctrl_boolean(tk, TK_PARENT, NULL, "FULLSCREEN: ");

                  // TODO: put a container here that is non-selectable
                  // add some text like:
                  // Fullscreen always uses the desktop resolution.
                  // Set a custom window resolution: ...
                  );

  return ret;
}
