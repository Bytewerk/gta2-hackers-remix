#include "../../../common/headers/common.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"

tk_screen_t *ui_screen_options(tk_t *tk, ui_t *ui) {
  tk_screen_t *options = tk_screen(tk, ui->credits, NULL);

  TK_STACK_SCREEN(
      options,

      TK_PARENT->bg_mashup = bg_mashup(tk->bg, NULL, "1_options", "1", NULL);

      tk_el_padding(TK_PARENT, 300, 250, 0, 0);

      tk_ctrl_button(tk, TK_PARENT, "VIDEO", NULL, ui->opt_video);
      tk_ctrl_button(tk, TK_PARENT, "AUDIO", NULL, ui->opt_audio);
      tk_el_disabled(tk_ctrl_button(tk, TK_PARENT, "CONTROLS", NULL, NULL));
      tk_ctrl_button(tk, TK_PARENT, "UPDATE NOTIFICATIONS", NULL,
                     ui->first_run);

      TK_PARENT->bottom_text_high = "CONTROLS SETUP INFO:";
      TK_PARENT->bottom_text_low = "VISIT GIT.IO/G2HR_CONTROLS";

      );

  return options;
}
