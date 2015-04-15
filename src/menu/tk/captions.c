#include "../sty/sty.h"
#include "toolkit.h"

void captions_draw_buttom_text(tk_t *tk) {
  tk_el_t *el = tk->screen_active->el_selected;
  if (!el || !el->bottom_text_low)
    el = &(tk->screen_active->el);

  for (char is_low = 0; is_low < 2; is_low++) {
    const char *text = is_low ? el->bottom_text_low : el->bottom_text_high;
    if (!text)
      continue;
    sty_text(tk->renderer, tk->fsty, GTA2_FONT_FSTYLE_WHITE_BLACK_TINY,
             0xffffffff, 300, 440 + is_low * 20, text);
  }
}
