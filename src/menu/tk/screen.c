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
                    int offset_y, char all_selected) {
  while (el) {
    char is_selected = all_selected || (el == el_selected);
    char is_disabled = el->flags & TK_EL_FLAG_DISABLED;
    int width = 0;
    int height = 0;

    offset_x += el->padding_left;
    offset_y += el->padding_top;

    // alpha and RGB overrides
    uint32_t argb = 0xffffffff;
    if (is_disabled && el->argb_disabled)
      argb = el->argb_disabled;
    if (is_selected && el->argb_selected)
      argb = el->argb_selected;

    if (el->type == LABEL) {
      char font = is_selected ? GTA2_FONT_FSTYLE_RED_BLACK_NORMAL
                              : GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;
      width = sty_text(tk->renderer, tk->fsty, font, argb, offset_x, offset_y,
                       el->text);
      height = 8;
    }
    if (el->type == SPRITE) {
      sty_sprite_draw(tk->renderer, tk->fsty, el->sprite_id, offset_x, offset_y,
                      el->width, el->height, argb);
      width = el->width;
      height = el->height;
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

  // make sure, that there's a control selected
  if (!screen->el_selected)
    screen->el_selected = screen->el.sub;

  // draw background
  tk_screen_draw_bg(tk);

  // draw all elements (and therefore controls)
  recursive_draw(tk, screen->el_selected, screen->el.sub,
                 screen->el.padding_left, screen->el.padding_top, 0);

  // draw bottom text
  captions_draw_buttom_text(tk);
}
