#include "toolkit.h"
#include <stdio.h>

void up(tk_screen_t *screen) {
  tk_el_t *selected = screen->el_selected;
  tk_el_t *listpos = screen->el.sub;
  if (!selected)
    return;

  // first entry -> get the last one!
  if (selected == listpos)
    while (listpos->next)
      listpos = listpos->next;
  else
    while (listpos && listpos->next != selected)
      listpos = listpos->next;

  screen->el_selected = listpos;

  if (screen->el_selected->flags & TK_EL_FLAG_DISABLED)
    up(screen);
}

void tk_screen_actionfunc(tk_t *tk, tk_el_t *el, tk_action_t action) {
  tk_screen_t *screen = tk->screen_active;

  if (action == TK_ACTION_UP)
    up(screen);

  if (action == TK_ACTION_DOWN) {
  }
}
