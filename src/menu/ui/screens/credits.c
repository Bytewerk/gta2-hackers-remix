#include "../../../common/common.h"
#include "../../gxt/gxt.h"
#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../ui.h"
#include "../ui_text.h"

#define CREDITS_MANUAL_SCROLL_OFFSET 50

typedef struct {
  tk_el_t *scrolling;
  ui_t *ui;
} ud_credits_t;

uint32_t credits_argb(char color) {
  switch (color) {
  case 'W':
    return 0xffffffff; // white
  case 'C':
    return 0xffffee00; // yellow
  case 'B':
    return 0xff52ceff; // cyan
  case 'G':
    return 0xff52ce00; // green
  }

  printf("credits - unknown color char: %c\n", color);
  return 0x00000000;
}

// TODO: Quit at the end of the credits? or at least stop scrolling.
void credits_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                        tk_action_t action) {
  ud_credits_t *ud = (ud_credits_t *)(el->userdata);
  ui_t *ui = ud->ui;

  if (action == TK_ACTION_BACKSPACE || action == TK_ACTION_ESC ||
      action == TK_ACTION_ENTER) {
    if (ui->slotmachine_enabled) {
      tk->screen_active = ui->main;
      ud->scrolling->scroll_top = 0;
    } else
      tk->quit = 1;
  }

  if (action == TK_ACTION_FRAMETIME) {
    ud->scrolling->scroll_top += 1;
    tk->redraw_needed = 1;
  }

  if (action == TK_ACTION_UP) {
    if (ud->scrolling->scroll_top > CREDITS_MANUAL_SCROLL_OFFSET)
      ud->scrolling->scroll_top -= CREDITS_MANUAL_SCROLL_OFFSET;
    else
      ud->scrolling->scroll_top = 0;
  }

  if (action == TK_ACTION_DOWN) {
    ud->scrolling->scroll_top += CREDITS_MANUAL_SCROLL_OFFSET;
  }
}

// there are lots of new line (*) characters at the start
// of the credits. this offset gets rid of them.
#define GTA2_CREDITS_OFFSET 20
void credits_add_original(tk_t *tk, tk_el_t *TK_PARENT) {
  char *original = gxt_get(tk->gxt, "credits");
  uint16_t original_length = strlen(original);
  uint16_t line_start = 0;
  char color = 'W';
  int offset_y = 0;

  for (int i = GTA2_CREDITS_OFFSET; i < original_length; i++) {
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
      } else
        offset_y += 10;
      line_start = i + 1;
    } else if (c == '#') // new color
    {
      color = original[i + 1];
      line_start = i + 2;
      i++;
    }
  }
}

#define ADD(FONT, PADDING_TOP, PADDING_BOTTOM, TEXT)                           \
  {                                                                            \
    tk_el_t *el = tk_label(tk, TK_PARENT, TEXT, FONT, 0);                      \
    el->padding_top = PADDING_TOP;                                             \
    el->padding_bottom = PADDING_BOTTOM;                                       \
    tk_el_center(el);                                                          \
  }

tk_screen_t *ui_screen_credits(tk_t *tk, ui_t *ui) {
  ud_credits_t *ud = malloc(sizeof(ud_credits_t));
  ud->ui = ui;

  tk_screen_t *credits = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      credits,
      // override default screen actionfunc. this is necessary,
      // because we never have a selected element!
      TK_PARENT->actionfunc = (void *)credits_actionfunc;
      TK_PARENT->userdata = (void *)ud;

      TK_PARENT->bg_mashup = bg_mashup(tk->bg, "credits", NULL, NULL, NULL);

      tk_el_padding(TK_PARENT, 120, 150, 0, 120); tk_el_width(TK_PARENT, 400);

      tk_el_t *hr_text =
          tk_label(tk, TK_PARENT, "HACKER'S REMIX V" G2HR_VERSION,
                   GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
      hr_text->padding_bottom = 5;
      // hr_text->argb_normal = credits_argb('C');
      tk_el_center(hr_text);

      // scrolling part
      TK_STACK(ud->scrolling = TK_PARENT; tk_el_scrollable(TK_PARENT);

               // G2HR credits
               // ...

               // original credits
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE, 200, 0, "ORIGINAL");
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE, 0, 20, "GTA2 CREDITS");
               credits_add_original(tk, TK_PARENT);));

  return credits;
}

#undef ADD
