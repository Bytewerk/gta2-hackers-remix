#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

// TODO: top right should say: N GAMEPADS CONNECTED
// TODO: more controls
// TODO: custom background!

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui) {
  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);
  TK_STACK_SCREEN(
      splitscreen,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "3_tables", NULL, NULL, NULL);
      tk_el_padding(TK_PARENT, 300, 161, 0, 0);

      tk_ctrl_circle(tk, TK_PARENT, "SCREEN LAYOUT", NULL, '1', '4', '1', NULL);

      );
  return splitscreen;
}
