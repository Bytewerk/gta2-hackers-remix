#pragma once

/*
        BUTTON:
                goes to another screen or executes a function.
*/
tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen,
                        void *onclick_func);

/*
        CIRCLE:
                < (A) >

        value:
                ((ud_circle_t*) ctrl->userdata)->value_str[0];
*/
tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char min2,
                        char max2, char value, void *actionfunc);

/*
        ARROWTEXT:
                < PLAYER 0 >
                  PLAYE_

        value:
                ud_arrowtext_t* ud = (ud_arrowtext_t*) ctrl->userdata;
                uint16_t id  = ud->entry_selected;
                char* string = ud->entries[id];
*/
tk_el_t *tk_ctrl_arrowtext(tk_t *tk, tk_el_t *TK_PARENT, bg_mashup_t *bg_mashup,
                           char **entries, uint16_t entry_count,
                           uint16_t entry_selected, char entry_length,
                           void *actionfunc, char editing_disabled,
                           char *bottom_text_high, char *bottom_text_low,
                           char *bottom_text_high_editing,
                           char *bottom_text_low_editing);

/*
        BOOLEAN (simplifies ARROWTEXT):
                < TEXT: DISABLED >

        value:
                ((ud_arrowtext_t*) ctrl->userdata)->entry_selected;
*/
tk_el_t *tk_ctrl_boolean(tk_t *tk, tk_el_t *TK_PARENT, const char *text,
                         bg_mashup_t *bg_mashup, char value);

/*
        Internal controls (used by other controls) and userdata.
        If you want to do fancy stuff, just cast 'n' access the userdata
        directly. No need for setters or getters here.
*/

// arrow
tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc, void *userdata);

// button
typedef struct {
  void *onclick_func;
  tk_screen_t *onclick_screen;
} ud_button_t;

// circle
typedef struct {
  char value_str[2]; // {value, '\0'}
  char min;
  char max;
  char min2;
  char max2;
  tk_el_t *container;
  tk_el_t *button;
  tk_el_t *left;
  tk_el_t *right;
  tk_el_t *circle_sprite;
  tk_el_t *circle_text;
  void *actionfunc;
} ud_circle_t;

// arrowtext
#define UNDERSCORE_BLINK_FRAMES 5
typedef struct {
  uint16_t entry_count;
  uint16_t entry_selected;
  char entry_length;
  char **entries;
  char underscore_frame_count;
  void *actionfunc;
  char editing_disabled;
  char *bottom_text_high;
  char *bottom_text_low;
  char *bottom_text_high_editing;
  char *bottom_text_low_editing;
  tk_el_t *container;
  tk_el_t *left;
  tk_el_t *text;
  tk_el_t *underscore;
  tk_el_t *right;
} ud_arrowtext_t;

// boolean
typedef struct {
  char **entries;
  tk_el_t *arrowtext;
} ud_boolean_t;
