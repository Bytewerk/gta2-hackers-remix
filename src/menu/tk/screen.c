#include "toolkit.h"

tk_screen_t *tk_screen(tk_t *tk) {
  tk_screen_t *screen = calloc(1, sizeof(tk_screen_t));

  // attach it to the tk for easy cleanup
  tk_el_t *listpos = (tk_el_t *)tk->screen_first;
  if (!listpos)
    tk->screen_first = screen;
  else {
    while (listpos->next)
      listpos = listpos->next;
    listpos->next = (tk_el_t *)screen;
  }
  return screen;
}

void tk_screen_draw(tk_t *tk) { printf("redrawing screen...\n"); }
