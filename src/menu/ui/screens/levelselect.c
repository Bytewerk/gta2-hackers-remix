#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include "../ui_text.h"

/*
        TODO:
                - show a detailed map description, author, date, ...
                - switch the container between the two panels when pressing
                        tab
                - add a red line between the panels
                - make the filters usable
                - change background, based on the selected levels
                - add F5 action, that reloads the mmp files and redraws the
                        screen
*/

typedef struct {
  tk_screen_t *screen;
  int entries_count;
  int index_last;
  tk_el_t *list;
  ui_t *ui;
} ud_levels_t;

void levels_set_scrolling(tk_t *tk, ud_levels_t *ud, tk_el_t *el_selected) {
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

  if (index <= elements_per_page / 2)
    ud->list->scroll_top = 0;
  else if (index >= ud->entries_count - elements_per_page / 2)
    ud->list->scroll_top =
        (ud->entries_count - elements_per_page) * el_full_height;
  else
    ud->list->scroll_top = (index - elements_per_page / 2) * el_full_height;

  // this also shouldn't be necessary!
  if (index != ud->index_last)
    tk->redraw_needed = 1;
  ud->index_last = index;
}

void levels_actionfunc_list(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                            tk_action_t action, SDL_Keycode key) {
  ud_levels_t *ud = (ud_levels_t *)el->userdata;

  if (action == TK_ACTION_BEFORE_FIRST_SCREEN_FRAME) {
    // sync the map selection
    char *map_name = ud->ui->maps[ud->ui->map_selected];
    tk_el_t *current = ud->list->sub;
    while (current) {
      char *map_name_current = cfg_read(((mmp_file_t *)current->userdata)->data,
                                        "MapFiles/Description");
      if (map_name_current == map_name)
        break;
      current = current->next;
    }

    ud->screen->el_selected = el_selected = current;
    levels_set_scrolling(tk, ud, el_selected);
  }

  // FIXME! listen to UP/DOWN actions instead - but fix the execution
  // order of the actionfunc's first (otherwise this will always
  // lag one frame behind!)
  if (action == TK_ACTION_FRAMETIME) {
    levels_set_scrolling(tk, ud, el_selected);
  }
}

void levels_actionfunc_label(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                             tk_action_t action, SDL_Keycode key) {
  if (action == TK_ACTION_ENTER && el == el_selected) {
    ud_levels_t *ud = (ud_levels_t *)el->parent->userdata;
    mmp_file_t *file = (mmp_file_t *)el->userdata;
    char *map_name = cfg_read(file->data, "MapFiles/Description");
    char **ui_maps = ud->ui->maps;

    // find the currently selected map inside the ui_maps array,
    // which might have a different sorting than the ui->mmp struct
    // in the future (eg. when the mmp struct gets sorted by author)
    // NOTE: comparing pointers in the if()!
    size_t count = ud->ui->mmp->file_count;
    size_t i = 0;
    for (; i < count; i++)
      if (ui_maps[i] == map_name)
        break;
    ud->ui->map_selected = i;

    // go back to the "splitscreen"-screen
    tk->screen_active = ud->ui->splitscreen;
  }
}

tk_screen_t *ui_screen_levels(tk_t *tk, ui_t *ui) {
  tk_screen_t *levels = tk_screen(tk, NULL, NULL);
  ud_levels_t *ud = malloc(sizeof(ud_levels_t));
  ud->entries_count = 0;
  ud->index_last = 0;
  ud->ui = ui;
  ud->screen = levels;

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
      TK_STACK(
          TK_PARENT->actionfunc = (void *)levels_actionfunc_list;
          TK_PARENT->userdata = (void *)ud; ud->list = TK_PARENT;

          levels->el_content_container = TK_PARENT;
          tk_el_padding(TK_PARENT, 300, 123, 0, 30);
          tk_el_height(TK_PARENT, 250); tk_el_scrollable(TK_PARENT);

          mmp_t *mmp = ui->mmp; for (size_t i = 0; i < mmp->file_count; i++) {
            // NOTE: the format printed in the label
            // doesn't need to be the map name. we have
            // the userdata for that.
            tk_el_t *label =
                tk_label(tk, TK_PARENT,
                         cfg_read(mmp->files[i]->data, "MapFiles/Description"),
                         GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
                         GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);
            label->userdata = (void *)mmp->files[i];
            label->actionfunc = levels_actionfunc_label;
            tk_el_selectable(label);
            ud->entries_count++;
          })

      /*
      // filter controls
      // to get these working, we'll need to add more info to the MMP
      // files (eg. authors, tags (such as: race map, ...), date, ...)

      TK_STACK
      (
              tk_el_padding(TK_PARENT, 300, 0, 0, 0);

              tk_ctrl_button(tk, TK_PARENT, "TAGS: ALL", NULL, NULL);
              tk_ctrl_button(tk, TK_PARENT, "ORDER BY: TITLE", NULL, NULL);
              tk_ctrl_button(tk, TK_PARENT, "FILTER: NONE", NULL, NULL);
      )
      */
      );

  return levels;
}
