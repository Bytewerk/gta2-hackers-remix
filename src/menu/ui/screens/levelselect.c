#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui) {
  tk_screen_t *levels = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      levels, tk_el_padding(TK_PARENT, 300, 250, 0, 0);

      // list all levels
      mmp_t *mmp = ui->mmp;
      for (size_t i = 0; i < mmp->file_count; i++) {
        tk_ctrl_button(tk, TK_PARENT,
                       mmp_read(mmp->files[i], "MapFiles/Description"), NULL,
                       NULL, NULL);
      }

      TK_PARENT->bottom_text_low = "G2HR V0.4";);

  return levels;
}
