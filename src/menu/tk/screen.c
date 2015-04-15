#include "../sty/sty.h"
#include "toolkit.h"
#include <SDL2/SDL.h>

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

void recursive_draw(tk_t *tk, tk_el_t *el_selected, tk_el_t *el, int offset_x,
                    int offset_y) {
  while (el) {
    if (el->type == LABEL) {
      // FIXME: change sty_text, so that it accepts offsets instead
      // of SDL_Rect

      SDL_Rect dest = {offset_x, offset_y};
      sty_text(tk->renderer, tk->fsty,
               GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, // FIXME: selected?
               0xffffffff, dest, el->text);
    }
    if (el->type == SPRITE) {
    }
    if (el->sub) {
      recursive_draw(tk, el_selected, el->sub, offset_x, offset_y);
    }

    // TODO: increase offset - depending on whether
    // the parent is a STACK or FLOW

    el = el->next;
  }
}

void tk_screen_draw(tk_t *tk) {
  tk_screen_t *screen = tk->screen_active;

  // draw background

  // draw all elements (and therefore controls)
  recursive_draw(tk, screen->el_selected, screen->el.sub,
                 screen->el.padding_left, screen->el.padding_top);
}
