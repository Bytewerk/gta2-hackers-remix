#include "controls.h"
#include "toolkit.h"

// BUTTON
typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
} ud_button_t;

// TODO: add selected element as parameter to actionfunc!
void actionfunc(tk_t *tk, tk_el_t *el, tk_action_t action) {
  tk_el_t *el_selected = tk->screen_active->el_selected;
  if (action != TK_ACTION_ENTER || el != el_selected)
    return;

  ud_button_t *ud = (ud_button_t *)el->userdata;
  // if(ud->onclick_func) - TODO

  if (ud->onclick_screen)
    tk->screen_active = ud->onclick_screen;
}

void tk_ctrl_button(tk_el_t *TK_PARENT, char *text, bg_mashup_t *bg_mashup,
                    tk_screen_t *onclick_screen, void *onclick_func) {
  tk_el_t *label = tk_label(TK_PARENT, text);

  ud_button_t *ud = malloc(sizeof(ud_button_t));
  ud->onclick_func = onclick_func;
  ud->onclick_screen = onclick_screen;
  label->bg_mashup = bg_mashup;
  label->userdata = (void *)ud;
  label->actionfunc = (void *)actionfunc;
}
