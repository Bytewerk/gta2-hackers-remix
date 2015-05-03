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

void tk_el_clean(tk_t *tk, tk_el_t *el) {
  while (el) {
    if (el->sub)
      tk_el_clean(tk, el->sub);
    tk_el_t *el_old = el;
    el = el->next;

    if (el_old->flags & TK_EL_FLAG_FREE_TEXT)
      free(el_old->text);

    // call clean up action
    if (el_old->actionfunc && el_old->userdata)
      tk_actions_element(tk, el_old, NULL, TK_ACTION_CLEANUP, SDLK_UNKNOWN);

    free(el_old);
  }
}

void tk_el_geocalc(tk_t *tk, tk_el_t *el, char /*bool*/ down,
                   char /*bool*/ up) {
  tk->redraw_needed = 1;
  if (down) {
    if (el->sub) {
      tk_el_geocalc(tk, el->sub, 1, 0);
      return;
    } else
      up = 1;
  }

  // actually caluclate the geometry here, depending on type
  if ((!(el->flags & TK_EL_FLAG_FORCE_WIDTH) ||
       !(el->flags & TK_EL_FLAG_FORCE_HEIGHT)) &&
      el->type != HIDDEN) {
    int width;
    int height;

    if (el->type == SPRITE)
      sty_sprite_measure(tk->fsty, &width, &height, el->sprite_id);
    else if (el->type == LABEL)
      sty_text_measure(tk->fsty, &width, &height, el->font_id, el->text);
    else if (el->type == STACK || el->type == FLOW) {
      width = 0;
      height = 0;
      tk_el_t *sub = el->sub;
      while (sub) {
        int el_width = sub->width + sub->padding_left + sub->padding_right;

        int el_height = sub->height + sub->padding_top + sub->padding_bottom;

        if (el->type == STACK) {
          width = width > el_width ? width : el_width;
          height += el_height;
        } else // FLOW
        {
          width += el_width;
          height = height > el_height ? height : el_height;
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

// returns 1 if it is selected, 0 if not
// set recursing to 0!
char tk_is_selected_recursive(tk_el_t *el, tk_el_t *el_selected,
                              char recursing) {
  while (el_selected) {
    if (el_selected == el)
      return 1;
    if (el_selected->sub && tk_is_selected_recursive(el, el_selected->sub, 1))
      return 1;

    if (!recursing)
      break;
    el_selected = el_selected->next;
  }
  return 0;
}

/*
        Extended elements
*/

tk_el_t *tk_sprite(tk_t *tk, tk_el_t *parent, char sprite_id) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, SPRITE);
  el->sprite_id = sprite_id;
  tk_el_geocalc(tk, el, 0, 0);
  return el;
}

tk_el_t *tk_label(tk_t *tk, tk_el_t *parent, char *text, char font_id,
                  char font_id_selected) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, LABEL);
  el->text = text;
  el->argb_disabled = 0x9cffffff;
  el->font_id = font_id;
  el->font_id_selected = font_id_selected;
  tk_el_geocalc(tk, el, 0, 0);
  return el;
}

/*
        Event: attach an actionfunc without any visible element
*/

tk_el_t *tk_event(tk_t *tk, tk_el_t *parent, void *actionfunc, void *userdata) {
  tk_el_t *el = calloc(1, sizeof(tk_el_t));
  tk_el_attach(el, parent, HIDDEN);
  el->actionfunc = actionfunc;
  el->userdata = userdata;
  return el;
}
