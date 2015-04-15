#include "elements.h"
#include <stdlib.h>

/*
        el: must be calloc'd
        parent: optional
*/
tk_el_t *tk_el_attach(tk_el_t *el, tk_el_t *parent, tk_el_type_t type) {
  el->type = type;
  el->parent = parent;

  if (parent) {
    if (parent->sub) {
      tk_el_t *prev = parent->sub;
      while (prev->next)
        prev = prev->next;

      prev->next = el;
    } else
      parent->sub = el;
  }

  return el;
};

void tk_el_clean(tk_el_t *el) {
  while (el) {
    if (el->sub)
      tk_el_clean(el->sub);
    tk_el_t *el_old = el;
    el = el->next;
    free(el_old);
  }
}

// FIXME: STUB!
void tk_el_geocalc(tk_el_t *el, char /*bool*/ down, char /*bool*/ up) {
  if (down && el->sub) {
    tk_el_geocalc(el->sub, 1, 0);
    return;
  } else
    up = 1;

  // TODO:
  // actually caluclate the geometry here, depending on type
  // and flags (forced width, height?)

  if (up && el->parent)
    tk_el_geocalc(el->parent, 0, 1);
}

/*
        Extended elements
*/

tk_el_t *tk_sprite(tk_el_t *parent, uint16_t sprite_id) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, SPRITE);
  el->sprite_id = sprite_id;
  tk_el_geocalc(el, 0, 0);
  return el;
}

tk_el_t *tk_label(tk_el_t *parent, char *text) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, LABEL);
  tk_el_padding(el, 0, 0, 0, 12);
  el->text = text;
  tk_el_geocalc(el, 0, 0);
  return el;
}
