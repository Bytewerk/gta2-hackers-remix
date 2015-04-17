#include "toolkit.h"

tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen,
                        void *onclick_func);

tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc);

tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char value,
                        void *actionfunc);
