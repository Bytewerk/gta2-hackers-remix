#include "toolkit.h"
#include <stdio.h>

void tk_screen_actionfunc(tk_t *tk, tk_el_t *el, tk_action_t action) {
  printf("action: %i\n", action);
}
