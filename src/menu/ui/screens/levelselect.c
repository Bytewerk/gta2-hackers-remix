#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

/*
        TODO:
                - make the map list scrollable (right now it is overflowing!)
                - switch the container between the two panels when pressing
                        tab
                - add a red line between the panels
                - make the filters usable
                - change background, based on the selected levels
                - add a function to each button, to put it back somehow
                - add F5 action, that reloads the mmp files and redraws the
                        screen
*/

tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui) {
  tk_screen_t *levels = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      levels,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "3_tables", NULL, NULL, NULL);

      // title bar
      tk_el_t *titlebar = tk_label(tk, TK_PARENT, "CHOOSE A MAP",
                                   GTA2_FONT_FSTYLE_WHITE_RED_NORMAL, 0);
      tk_el_padding(titlebar, 315, 12, 0, 0);

      // map list
      TK_STACK(levels->el_content_container = TK_PARENT;

               tk_el_padding(TK_PARENT, 300, 123, 0, 30);
               tk_el_height(TK_PARENT, 200);

               mmp_t *mmp = ui->mmp;
               for (size_t i = 0; i < mmp->file_count; i++) {
                 for (int n = 0; n < 4; n++) // debug: more entries to scroll
                 {
                   tk_el_t *el = tk_ctrl_button(
                       tk, TK_PARENT,
                       mmp_read(mmp->files[i], "MapFiles/Description"), NULL,
                       NULL, NULL);
                   el->userdata = (void *)mmp->files[i];
                 }
               })

      // filter controls
      TK_STACK(
          tk_el_padding(TK_PARENT, 300, 0, 0, 0);

          tk_ctrl_button(tk, TK_PARENT, "TAGS: ALL", NULL, NULL, NULL);
          tk_ctrl_button(tk, TK_PARENT, "ORDER BY: TITLE", NULL, NULL, NULL);
          tk_ctrl_button(tk, TK_PARENT, "FILTER: NONE", NULL, NULL, NULL);)

          TK_PARENT->bottom_text_low = "PRESS H FOR HELP";);

  return levels;
}
