#include "toolkit.h"
#include <stdio.h>

void up(tk_screen_t *screen, tk_el_t *selected, tk_el_t *first) {
  tk_el_t *listpos = first;

  // first entry -> get the last one!
  if (selected == listpos)
    while (listpos->next)
      listpos = listpos->next;
  else
    while (listpos && listpos->next != selected)
      listpos = listpos->next;

  screen->el_selected = listpos;

  if (screen->el_selected->flags & TK_EL_FLAG_DISABLED)
    up(screen, screen->el_selected, first);
}

void down(tk_screen_t *screen, tk_el_t *selected, tk_el_t *first) {
  screen->el_selected = selected->next ? selected->next : first;

  if (screen->el_selected->flags & TK_EL_FLAG_DISABLED)
    down(screen, screen->el_selected, first);
}

void back(tk_t *tk, tk_screen_t *screen) {
  if (screen->back)
    tk->screen_active = screen->back;
}

void tk_screen_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action) {
  tk_screen_t *screen = tk->screen_active;
  tk_el_t *first = screen->el.sub;

  if (el_selected) {
    if (action == TK_ACTION_UP)
      up(screen, el_selected, first);
    if (action == TK_ACTION_DOWN)
      down(screen, el_selected, first);
    if (action == TK_ACTION_BACK)
      back(tk, screen);
  }

  if (screen->actionfunc) {
    void (*actionfunc)(tk_t *, tk_el_t *, tk_el_t *, tk_action_t) =
        screen->actionfunc;
    if (actionfunc)
      actionfunc(tk, el, el_selected, action);
  }
}
