#include "../../../common/headers/common.h"
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
    return GTA2_FONT_COLOR_WHITE;
  case 'C':
    return GTA2_FONT_COLOR_YELLOW;
  case 'B':
    return GTA2_FONT_COLOR_CYAN;
  case 'G':
    return GTA2_FONT_COLOR_GREEN;
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
    if (ui->slotmachine) {
      tk->screen_active = ui->main;
      ud->scrolling->scroll_top = 0;
      sfx_play_song(ui->tk->sfx, SFX_SONG_MAINMENU);
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

  if (action == TK_ACTION_BEFORE_FIRST_SCREEN_FRAME) {
    sfx_play_song(ui->tk->sfx, SFX_SONG_CREDITS);
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

#define ADD(FONT, PADDING_TOP, PADDING_BOTTOM, TEXT, COLOR)                    \
  {                                                                            \
    tk_el_t *el = tk_label(tk, TK_PARENT, TEXT, FONT, 0);                      \
    el->padding_top = PADDING_TOP;                                             \
    el->padding_bottom = PADDING_BOTTOM;                                       \
    el->argb_normal = COLOR;                                                   \
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
      tk_el_center(hr_text);

      tk_el_t *url = tk_label(tk, TK_PARENT, "G2HR.ORG",
                              GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0);
      url->padding_bottom = 10; tk_el_center(url);

      // scrolling part
      TK_STACK(ud->scrolling = TK_PARENT; tk_el_scrollable(TK_PARENT);

               /*
                       G2HR CREDITS
               */
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 200, 0, "CREATED BY",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "ROBOTANARCHY",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "ROBOTANARCHY.SPACE", GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 20, 0, "AS A",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "BYTEWERK",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "PROJECT",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0, "BYTEWERK.ORG",
                   GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 30, 0,
                   "PRO-TIP: PRESS UP/DOWN TO", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "SCROLL THESE CREDITS", GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "BASED ON LOTS OF", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "ASM PATCHING BY",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "VIKE THE HUBE",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0, "GTAMP.COM/GTA2",
                   GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "AUDIO, SPRITES AND", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "FONT PARSING BASED", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "ON OPENGBH/OPENGTA2", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "TOOLS BY",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "BLACK PHOENIX",
                   GTA2_FONT_COLOR_CYAN);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "VERY USEFUL GTA2", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "TOOLS, RESOURCES, FORUMS", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "AND MEMORY ADDRESSES", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "SEKTOR",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0, "GTAMP.COM",
                   GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "LOTS OF HELP WITH", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "PORTING THE PROXYDLL", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "CODE TO MINGW",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "JESTER",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "JESTER01 AT FREENODE IRC", GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0, "SOME SMART",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "AUTOIT3/WINAPI HACKS", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "SQOZZ",
                   GTA2_FONT_COLOR_CYAN);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "TESTING AND FEEDBACK", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "CUBAN-PETE",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "CFR34K",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "RANOMIER",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "SEKTOR",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "SQOZZ",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "THEBLACKCROW1",
                   GTA2_FONT_COLOR_CYAN);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "DUDE WHO BOUGHT THE", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "OFFICIAL G2HR SLOT", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "MACHINE FOR ONE EURO", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "FROM THE INTERNET", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "JUHE",
                   GTA2_FONT_COLOR_CYAN);

               /*
                       G2HR CREDITS: MENU ARTWORK SECTION
               */
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0, "MENU ARTWORK",
                   GTA2_FONT_COLOR_GREEN);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "FINDING AND EDITING OF", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0,
                   "COOL CC-LICENSED PICS", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "CUBAN-PETE",
                   GTA2_FONT_COLOR_CYAN);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0,
                   "GUNS AND PEARLS", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "MAKEUPSIREN",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "MAKEUPSIREN.DEVIANTART.COM", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "LICENSE: CC BY 3.0", GTA2_FONT_COLOR_WHITE);

               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 30, 0, "UNTITLED",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0, 0, "STUART MCALPINE",
                   GTA2_FONT_COLOR_CYAN);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "STUARTMCALPINE AT FLICKR", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "LICENSE: CC BY 2.0", GTA2_FONT_COLOR_WHITE);

               /*
                       FREE SOFTWARE, YO!
               */
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 30, 0,
                   "REMEMBER: YOUR NAME COULD", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "BE ALONG THESE LINES. IT'S", GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 0,
                   "FREE SOFTWARE, JUST FORK IT!", GTA2_FONT_COLOR_WHITE);

               /*
                       ORIGINAL CREDITS
               */
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE, 40, 0, "ORIGINAL",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE, 0, 0, "GTA2 CREDITS",
                   GTA2_FONT_COLOR_WHITE);
               ADD(GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0, 40,
                   "FROM THE PREVIOUS MILLENIUM", GTA2_FONT_COLOR_WHITE);
               credits_add_original(tk, TK_PARENT);));

  return credits;
}

#undef ADD
