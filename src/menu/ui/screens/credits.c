#include "../../gxt/gxt.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

#define CREDITS_MANUAL_SCROLL_OFFSET 50

uint32_t credits_argb(char color) {
  switch (color) {
  case 'W':
    return 0x00000000; // white
  case 'C':
    return 0xffffee00; // yellow
  case 'B':
    return 0xff52ceff; // cyan
                       // case 'G': return ;
  }

  printf("credits - unknown color char: %c\n", color);
  return 0x00000000;
}

// TODO: Quit at the end of the credits? or at least stop scrolling.
void actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                tk_action_t action) {
  if (action == TK_ACTION_BACKSPACE || action == TK_ACTION_ESC ||
      action == TK_ACTION_ENTER) {
    tk->quit = 1;
  }

  if (action == TK_ACTION_FRAMETIME) {
    el->scroll_top += 1;
    tk->redraw_needed = 1;
  }

  if (action == TK_ACTION_UP) {
    if (el->scroll_top > CREDITS_MANUAL_SCROLL_OFFSET)
      el->scroll_top -= CREDITS_MANUAL_SCROLL_OFFSET;
    else
      el->scroll_top = 0;
  }

  if (action == TK_ACTION_DOWN) {
    el->scroll_top += CREDITS_MANUAL_SCROLL_OFFSET;
  }
}

tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui) {
  tk_screen_t *credits = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      credits,
      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "credits", NULL, NULL, NULL);
      tk_el_padding(TK_PARENT, 120, 150, 0, 120); tk_el_width(TK_PARENT, 400);
      tk_el_scrollable(TK_PARENT); TK_PARENT->actionfunc = (void *)actionfunc;

      char *original = gxt_get(tk->gxt, "credits");
      uint16_t original_length = strlen(original); uint16_t line_start = 0;
      char color = 'W'; int offset_y = 0;

      for (int i = 0; i < original_length; i++) {
        char c = original[i];
        if (c == '*') // new line
        {
          if (line_start < i) {
            char *line = malloc(i - line_start + 1);
            strncpy(line, original + line_start, i - line_start);
            line[i - line_start] = '\0';

            tk_el_t *el = tk_label(tk, TK_PARENT, line,
                                   GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
            tk_el_center(el);
            tk_el_free_text(el);
            tk_el_padding(el, 0, offset_y, 0, 0);
            el->argb_normal = credits_argb(color);

            offset_y = 0;
          } else {
            offset_y += 10;
          }
          line_start = i + 1;
        } else if (c == '#') // new color
        {
          color = original[i + 1];
          line_start = i + 2;
          i++; // skip the color
        }
      });

  return credits;
}
