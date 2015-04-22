#include "../sty/sty.h"
#include "toolkit.h"
#include <SDL2/SDL.h>

// draw background function
#define DRAW_BG(WHERE, XPOS, YPOS, WIDTH, HEIGHT)                              \
  {                                                                            \
    if (mashup->WHERE) {                                                       \
      SDL_Rect dstrect = {XPOS, YPOS, WIDTH, HEIGHT};                          \
      SDL_Texture *tex =                                                       \
          SDL_CreateTextureFromSurface(tk->renderer, mashup->WHERE->surface);  \
      SDL_RenderCopy(tk->renderer, tex, NULL, &dstrect);                       \
      SDL_DestroyTexture(tex);                                                 \
    };                                                                         \
  }
void tk_screen_draw_bg(tk_t *tk) {
  bg_mashup_t *mashup = tk->screen_active->el.bg_mashup;
  tk_el_t *selected = tk->screen_active->el_selected;

  if (selected && selected->bg_mashup)
    mashup = selected->bg_mashup;
  if (!mashup)
    return;

  DRAW_BG(full, 000, 000, 640, 480);
  DRAW_BG(left, 000, 000, 278, 480);
  DRAW_BG(right, 278, 000, 362, 480);
  DRAW_BG(custom, 000, 000, 640, 480);
}
#undef DRAW_BG

tk_screen_t *tk_screen(tk_t *tk, tk_screen_t *back, void *actionfunc) {
  tk_screen_t *screen = calloc(1, sizeof(tk_screen_t));
  screen->el.type = STACK;
  screen->el.actionfunc = (void *)tk_screen_actionfunc;
  screen->back = back;
  screen->actionfunc = actionfunc;

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
                    int offset_y, int cutoff_y, char all_selected) {
  while (el) {
    if (offset_y > 480)
      break; // off screen!

    int offset_x_old = offset_x;
    char is_selected = all_selected || (el == el_selected);
    char is_disabled = el->flags & TK_EL_FLAG_DISABLED;

    // alpha and RGB overrides
    uint32_t argb = 0xffffffff;
    if (el->argb_normal)
      argb = el->argb_normal;
    if (is_disabled && el->argb_disabled)
      argb = el->argb_disabled;
    if (is_selected && el->argb_selected)
      argb = el->argb_selected;

    if (!(el->flags & TK_EL_FLAG_INVISIBLE)) {

      if (el->flags & TK_EL_FLAG_H_CENTER)
        offset_x += el->parent->width / 2 - el->width;

      if (el->type == LABEL) {
        char font = el->font_id;
        if (is_selected && el->font_id_selected)
          font = el->font_id_selected;
        sty_text(tk->renderer, tk->fsty, font, argb,
                 offset_x + el->padding_left, offset_y + el->padding_top,
                 cutoff_y - el->padding_top, el->text);
      }
      if (el->type == SPRITE) {
        sty_sprite_draw(tk->renderer, tk->fsty, el->sprite_id,
                        offset_x + el->padding_left, offset_y + el->padding_top,
                        cutoff_y - el->padding_top, el->width, el->height,
                        argb);
      }
      if (el->sub) {
        tk_el_t *sub = el->sub;
        int sub_offset_y = offset_y + el->padding_top;
        int sub_cutoff_y = 0;

        if (el->type == STACK && el->scroll_top) {
          // skip all elements that were scrolled off screen
          int skipped_height = 0;
          while (sub && sub->height + skipped_height <= el->scroll_top) {
            skipped_height += sub->height;
            sub = sub->next;
          }

          // calculate new offset and remaining cutoff value
          // sub_offset_y += skipped_height;
          sub_cutoff_y = el->scroll_top - skipped_height;
        }

        if (sub) {
          recursive_draw(tk, el_selected, sub, offset_x + el->padding_left,
                         sub_offset_y, sub_cutoff_y, is_selected);
        }
      }
    }

    offset_x = offset_x_old;

    if (el->parent) {
      if (el->parent->type == STACK)
        offset_y +=
            el->padding_top + el->height + el->padding_bottom - cutoff_y;
      else if (el->parent->type == FLOW)
        offset_x += el->padding_left + el->width + el->padding_right;
      else
        printf("ERROR in recursive_draw: Not in a FLOW or STACK!");
    } else
      break; // must be a screen. only draw one screen at once!

    cutoff_y = 0;
    el = el->next;
  }
}

void tk_screen_draw(tk_t *tk) {
  tk_screen_t *screen = tk->screen_active;

  // make sure, that there's a control selected
  if (!screen->el_selected)
    screen->el_selected = screen->el_content_container
                              ? screen->el_content_container->sub
                              : screen->el.sub;

  // draw background
  tk_screen_draw_bg(tk);

  // draw all elements (and therefore controls)
  recursive_draw(tk, screen->el_selected, &(screen->el), 0, 0, 0, 0);

  // draw bottom text
  captions_draw_buttom_text(tk);
}
