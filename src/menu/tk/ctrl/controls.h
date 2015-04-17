tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc, void *userdata);

tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen,
                        void *onclick_func);

tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char value,
                        void *actionfunc);

tk_el_t *tk_ctrl_arrowtext(tk_t *tk, tk_el_t *TK_PARENT, bg_mashup_t *bg_mashup,
                           char **entries, uint16_t entry_count,
                           uint16_t entry_selected, char entry_length,
                           void *actionfunc, char editing_disabled,
                           char *bottom_text_high, char *bottom_text_low,
                           char *bottom_text_high_editing,
                           char *bottom_text_low_editing);
