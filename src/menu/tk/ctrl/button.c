#include "../../sty/font.h"
#include "../toolkit.h"
#include "controls.h"

void button_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action, SDL_Keycode key) {
  // We have a tk_screen_t as userdata, which must not be free'd here.
  // the toolkit already handles the cleanup right.
  if (action == TK_ACTION_CLEANUP)
    el->userdata = NULL;

  if (action == TK_ACTION_ENTER && el == el_selected && el->userdata) {
    sfx_play_sample(tk->sfx, SFX_FSTYLE_ENTER);
    tk->screen_active = (tk_screen_t *)el->userdata;
  }
}

tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen) {
  tk_el_t *label =
      tk_label(tk, TK_PARENT, text, GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
               GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);
  label->bg_mashup = bg_mashup;
  label->userdata = onclick_screen;
  label->actionfunc = (void *)button_actionfunc;
  tk_el_selectable(label);

  return label;
}
