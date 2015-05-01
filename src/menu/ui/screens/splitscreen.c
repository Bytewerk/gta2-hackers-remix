#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

/*
        TODO: replace all the buttons with functional controls
*/

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui, tk_screen_t *levels) {
  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);
  TK_STACK_SCREEN(
      splitscreen,

      TK_PARENT->bg_mashup =
          bg_mashup(tk->bg, "3_tables", NULL, NULL, "g2hr_splitscreen");

      // title bar
      tk_el_t *titlebar = tk_label(tk, TK_PARENT, "4 CONTROLLERS FOUND",
                                   GTA2_FONT_FSTYLE_WHITE_RED_NORMAL, 0);
      tk_el_padding(titlebar, 315, 12, 0, 0);

      // content
      TK_STACK(
          splitscreen->el_content_container = TK_PARENT;
          tk_el_padding(TK_PARENT, 300, 170, 0, 0);

          tk_ctrl_button(tk, TK_PARENT, "PLAYERS: 3", NULL, NULL, NULL);

          tk_ctrl_button(tk, TK_PARENT, "SCREEN LAYOUT: A", NULL, NULL, NULL);

          tk_ctrl_button(tk, TK_PARENT, "MAP: TINY TOWN", NULL, levels, NULL);

          tk_ctrl_button(tk, TK_PARENT, "GAME TYPE: FRAGS", NULL, NULL, NULL);

          tk_ctrl_button(tk, TK_PARENT, "TIME: 5 MIN", NULL, NULL, NULL);

          tk_ctrl_boolean(tk, TK_PARENT, "COPS", NULL, 1);

          // big play button. We don't have the huge font in red
          // for black backgrounds, but modifying argb_selected has
          // the same effect :)

          tk_el_t *play =
              tk_ctrl_button(tk, TK_PARENT, "PLAY!", NULL, NULL, NULL);
          play->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE;
          play->font_id_selected = GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE;
          play->argb_selected = 0xffff0000; tk_el_padding(play, 0, 20, 0, 0);

          );

      );
  return splitscreen;
}
