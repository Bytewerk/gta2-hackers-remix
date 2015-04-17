#include "elements.h"
#include "../sty/sty.h"
#include "toolkit.h"
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

void tk_el_geocalc(tk_t *tk, tk_el_t *el, char /*bool*/ down,
                   char /*bool*/ up) {
  if (down && el->sub) {
    tk_el_geocalc(tk, el->sub, 1, 0);
    return;
  } else
    up = 1;

  // actually caluclate the geometry here, depending on type
  if (!(el->flags & TK_EL_FLAG_FORCE_WIDTH) ||
      !(el->flags & TK_EL_FLAG_FORCE_HEIGHT)) {
    int width;
    int height;

    if (el->type == SPRITE)
      sty_sprite_measure(tk->fsty, &width, &height, el->sprite_id);
    else if (el->type == LABEL)
      sty_text_measure(tk->fsty, &width, &height,
                       GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, el->text);
    else if (el->type == STACK || el->type == FLOW) {
      width = 0;
      height = 0;
      tk_el_t *sub = el->sub;
      while (sub) {
        if (el->type == STACK) {
          width = width > sub->width ? width : sub->width;
          height += sub->height;
        } else // FLOW
        {
          width += sub->width;
          height = height > sub->height ? height : sub->height;
        }
        sub = sub->next;
      }
    }

    if (!(el->flags & TK_EL_FLAG_FORCE_WIDTH))
      el->width = width;

    if (!(el->flags & TK_EL_FLAG_FORCE_HEIGHT))
      el->height = height;
  }

  if (!el->width || !el->height || el->width > 640 || el->height > 480)
    printf("ERROR: invalid geometry: %ix%i (type %i)\n", el->width, el->height,
           el->type);

  if (up && el->parent)
    tk_el_geocalc(tk, el->parent, 0, 1);
}

/*
        Extended elements
*/

tk_el_t *tk_sprite(tk_t *tk, tk_el_t *parent, char sprite_id,
                   uint32_t argb_selected) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, SPRITE);
  el->sprite_id = sprite_id;
  el->argb_selected = argb_selected;
  tk_el_geocalc(tk, el, 0, 0);
  return el;
}

tk_el_t *tk_label(tk_t *tk, tk_el_t *parent, char *text) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, LABEL);
  el->text = text;
  el->argb_disabled = 0x9cffffff;
  tk_el_geocalc(tk, el, 0, 0);
  return el;
}
