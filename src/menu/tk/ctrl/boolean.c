#include "../../sty/font.h"
#include "../../sty/sprites.h"
#include "../toolkit.h"
#include "controls.h"

void bool_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                     tk_action_t action, SDL_Keycode key) {
  ud_boolean_t *ud = (ud_boolean_t *)el->userdata;

  if (action == TK_ACTION_CLEANUP) {
    free(ud->entries[0]);
    free(ud->entries[1]);
    free(ud->entries);
    free(ud);
  }
}

tk_el_t *tk_ctrl_boolean(tk_t *tk, tk_el_t *TK_PARENT, const char *text,
                         bg_mashup_t *bg_mashup, char value) {

  ud_boolean_t *ud = malloc(sizeof(ud_boolean_t));
  ud->entries = malloc(sizeof(char *) * 2);

  // Yes, this is a bit messy :p
  char *disabled = ": DISABLED";
  char *enabled = ": ENABLED";
  int textlen = strlen(text);
  ud->entries[0] = malloc(textlen + strlen(disabled) + 1);
  ud->entries[1] = malloc(textlen + strlen(enabled) + 1);
  strncpy(ud->entries[0], text, textlen);
  strncpy(ud->entries[0] + textlen, disabled, strlen(disabled) + 1);
  strncpy(ud->entries[1], text, textlen);
  strncpy(ud->entries[1] + textlen, enabled, strlen(enabled) + 1);

  tk_el_t *container;
  TK_FLOW(container = TK_PARENT; container->userdata = ud;
          container->actionfunc = bool_actionfunc;
          container->bg_mashup = bg_mashup; tk_el_selectable(container);

          ud->arrowtext = tk_ctrl_arrowtext(tk, TK_PARENT, NULL, ud->entries, 2,
                                            (value == 1), 0, NULL, 1, NULL,
                                            NULL, NULL, NULL););

  return container;
}
