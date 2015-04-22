#include "../../sty/font.h"
#include "../toolkit.h"
#include "controls.h"

void button_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action, SDL_Keycode key) {
  if (action != TK_ACTION_ENTER || el != el_selected)
    return;

  ud_button_t *ud = (ud_button_t *)el->userdata;
  // if(ud->onclick_func) - TODO

  if (ud->onclick_screen)
    tk->screen_active = ud->onclick_screen;
}

tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen,
                        void *onclick_func) {
  ud_button_t *ud = malloc(sizeof(ud_button_t));
  ud->onclick_func = onclick_func;
  ud->onclick_screen = onclick_screen;

  tk_el_t *label =
      tk_label(tk, TK_PARENT, text, GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL,
               GTA2_FONT_FSTYLE_RED_BLACK_NORMAL);
  label->bg_mashup = bg_mashup;
  label->userdata = (void *)ud;
  label->actionfunc = (void *)button_actionfunc;
  tk_el_selectable(label);

  return label;
}
