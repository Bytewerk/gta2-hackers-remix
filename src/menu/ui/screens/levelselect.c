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

typedef struct {
  int entries_count;
  tk_el_t *list;

} ud_levels_t;

void levels_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action, SDL_Keycode key) {
  ud_levels_t *ud = (ud_levels_t *)el->userdata;

  // set the scrolling right
  // FIXME: should work with action == UP, DOWN (less CPU time!),
  // but somehow it gets executed before the selected element
  // changes!
  if (action == TK_ACTION_FRAMETIME) {
    // find out the index of the selected element
    int index = 0;
    tk_el_t *current = ud->list->sub;
    while (current) {
      if (current == el_selected)
        break;
      current = current->next;
      index++;
    }

    int el_full_height = el_selected->padding_top + el_selected->height +
                         el_selected->padding_bottom;
    int elements_per_page = ud->list->height / el_full_height;

    // don't scroll when the very top elements are selected
    if (index <= elements_per_page / 2)
      return;

    ud->list->scroll_top = (index - elements_per_page / 2) * el_full_height;
  }
}

tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui) {
  tk_screen_t *levels = tk_screen(tk, NULL, NULL);
  ud_levels_t *ud = malloc(sizeof(ud_levels_t));
  ud->entries_count = 0;

  TK_STACK_SCREEN(
      levels,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "3_tables", NULL, NULL, NULL);

      // title bar
      tk_el_t *titlebar = tk_label(tk, TK_PARENT, "CHOOSE A MAP",
                                   GTA2_FONT_FSTYLE_WHITE_RED_NORMAL, 0);
      tk_el_padding(titlebar, 315, 12, 0, 0);

      // bottom text
      // TK_PARENT->bottom_text_low = "THIS LIST CAN BE SCROLLED";

      // map list
      TK_STACK(TK_PARENT->actionfunc = (void *)levels_actionfunc;
               TK_PARENT->userdata = (void *)ud; ud->list = TK_PARENT;

               levels->el_content_container = TK_PARENT;
               tk_el_padding(TK_PARENT, 300, 123, 0, 30);
               tk_el_height(TK_PARENT, 250); tk_el_scrollable(TK_PARENT);

               mmp_t *mmp = ui->mmp;
               for (size_t i = 0; i < mmp->file_count; i++) {
                 for (int n = 0; n < 4; n++) // debug: more entries to scroll
                 {
                   tk_el_t *el = tk_ctrl_button(
                       tk, TK_PARENT,
                       mmp_read(mmp->files[i], "MapFiles/Description"), NULL,
                       NULL, NULL);
                   el->userdata = (void *)mmp->files[i];
                   ud->entries_count++;
                 }
               })

      /*
      // filter controls
      // to get these working, we'll need to add more info to the MMP
      // files (eg. authors, tags (such as: race map, ...), date, ...)

      TK_STACK
      (
              tk_el_padding(TK_PARENT, 300, 0, 0, 0);

              tk_ctrl_button(tk, TK_PARENT, "TAGS: ALL", NULL, NULL,
                      NULL);
              tk_ctrl_button(tk, TK_PARENT, "ORDER BY: TITLE", NULL, NULL,
                      NULL);
              tk_ctrl_button(tk, TK_PARENT, "FILTER: NONE", NULL, NULL,
                      NULL);
      )
      */
      );

  return levels;
}
