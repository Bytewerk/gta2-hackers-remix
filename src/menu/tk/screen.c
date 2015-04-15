#include "../sty/sty.h"
#include "toolkit.h"
#include <SDL2/SDL.h>

tk_screen_t *tk_screen(tk_t *tk) {
  tk_screen_t *screen = calloc(1, sizeof(tk_screen_t));
  screen->el.type = STACK;

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
                    int offset_y, char all_selected) {
  while (el) {
    char is_selected = all_selected || (el == el_selected);
    int width = 0;
    int height = 0;

    offset_x += el->padding_left;
    offset_y += el->padding_top;

    if (el->type == LABEL) {
      char font = is_selected ? GTA2_FONT_FSTYLE_RED_BLACK_NORMAL
                              : GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;
      width = sty_text(tk->renderer, tk->fsty, font, 0xffffffff, offset_x,
                       offset_y, el->text);
      height = 8;
    }
    if (el->type == SPRITE) {
      // TODO!
    }
    if (el->sub) {
      recursive_draw(tk, el_selected, el->sub, offset_x, offset_y, is_selected);
    }

    if (el->parent->type == STACK)
      offset_y += el->padding_bottom + height;
    else if (el->parent->type == FLOW)
      offset_x += el->padding_right + width;
    else
      printf("ERROR in recursive_draw: Not in a FLOW or STACK!");

    el = el->next;
  }
}

void tk_screen_draw(tk_t *tk) {
  tk_screen_t *screen = tk->screen_active;

  // draw background

  // draw all elements (and therefore controls)
  recursive_draw(tk, screen->el_selected, screen->el.sub,
                 screen->el.padding_left, screen->el.padding_top, 0);
}
