#include "controls.h"
#include "toolkit.h"

// BUTTON
typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
  bg_mashup_t *bg_mashup;
} ud_button_t;

void action_button() {
  // if action == onclick ...
}

void tk_ctrl_button(tk_el_t *TK_PARENT, char *text, bg_mashup_t *bg_mashup,
                    tk_screen_t *onclick_screen, void *onclick_func) {
  tk_el_t *label = tk_label(TK_PARENT, text);

  ud_button_t *ud = malloc(sizeof(ud_button_t));
  ud->onclick_screen = onclick_screen;
  ud->onclick_func = onclick_func;
  ud->bg_mashup = bg_mashup;
  label->userdata = (void *)ud;
  label->actionfunc = (void *)action_button;
}
