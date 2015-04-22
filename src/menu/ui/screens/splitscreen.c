#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui) {
  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);
  TK_STACK_SCREEN(
      splitscreen,

      TK_PARENT->bg_mashup =
          bg_mashup(tk->bg, "3_tables", NULL, NULL, "g2hr_splitscreen");

      // title bar
      tk_el_t *titlebar = tk_label(tk, TK_PARENT, "4 CONTROLLERS FOUND");
      titlebar->font_id = GTA2_FONT_FSTYLE_WHITE_RED_NORMAL;
      tk_el_padding(titlebar, 315, 12, 0, 0);

      // content
      TK_STACK(splitscreen->el_content_container = TK_PARENT;
               tk_el_padding(TK_PARENT, 300, 123, 0, 0);

               tk_ctrl_circle(tk, TK_PARENT, "PLAYER COUNT", NULL, '1', '4', 0,
                              0, '3', NULL);

               tk_ctrl_circle(tk, TK_PARENT, "SCREEN LAYOUT", NULL, 'A', 'C', 0,
                              0, 'B', NULL);

               // STUB: this doesn't get free'd!
               char **entries = malloc(sizeof(char *) * 3);
               entries[0] = "FRAGS"; entries[1] = "POINTS"; entries[2] = "TAG";

               tk_ctrl_listbutton(tk, TK_PARENT, NULL, entries, 3, 1, 0,
                                  "GAME TYPE", 1, NULL, NULL, NULL, NULL);

               tk_ctrl_boolean(tk, TK_PARENT, "COPS", NULL, 1);

               );

      );
  return splitscreen;
}
