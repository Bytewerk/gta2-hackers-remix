#pragma once

/*
        BUTTON:
                goes to another screen or executes a function.
*/
tk_el_t *tk_ctrl_button(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, tk_screen_t *onclick_screen);

/*
        CIRCLE:

                text
                        < (A) >

        value:
                ((ud_circle_t*) ctrl->userdata)->value_str[0];
*/
tk_el_t *tk_ctrl_circle(tk_t *tk, tk_el_t *TK_PARENT, char *text,
                        bg_mashup_t *bg_mashup, char min, char max, char min2,
                        char max2, char value);

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
                           char editing_maxlen, char **entries,
                           uint16_t entry_count, char *prefix, char *suffix,
                           char *bottom_text_high, char *bottom_text_low,
                           char *bottom_text_high_editing,
                           char *bottom_text_low_editing);

// arrowtext consists of lots of sub-elements. use these
// helper functions to disable and enable them properly:
void tk_ctrl_arrowtext_disabled(tk_el_t *arrowtext_ctrl);
void tk_ctrl_arrowtext_enabled(tk_el_t *arrowtext_ctrl);

// tries to set the value mapped to the given string. if it can't be
// found, the value doesn't get changed.
void tk_ctrl_arrowtext_set_value_by_string(tk_el_t *ctrl, char *string);

// Macro that simplifies ARROWTEXT:
#define tk_ctrl_boolean(TK, TK_PARENT, BG_MASHUP, PREFIX)                      \
  tk_ctrl_arrowtext(TK, TK_PARENT, BG_MASHUP, 0, NULL, 0, PREFIX, NULL, NULL,  \
                    NULL, NULL, NULL);

/*
        Internal controls (used by other controls) and userdata.
        If you want to do fancy stuff, just cast 'n' access the userdata
        directly. No need for setters or getters here.
*/

// arrow
tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc, void *userdata);

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
} ud_circle_t;

// arrowtext
#define UNDERSCORE_BLINK_FRAMES 5
typedef struct {
  // constructor parameters
  char editing_maxlen;
  char **entries;
  uint16_t entry_count;
  char *prefix;
  char *suffix;
  char *bottom_text_high;
  char *bottom_text_low;
  char *bottom_text_high_editing;
  char *bottom_text_low_editing;

  // other
  uint16_t entry_selected;
  char underscore_frame_count;
  char free_bool_default_entries;
  tk_el_t *container;
  tk_el_t *left;
  tk_el_t *text_pre;
  tk_el_t *text;
  tk_el_t *text_suf;
  tk_el_t *underscore;
  tk_el_t *right;
} ud_arrowtext_t;

// boolean
typedef struct {
  char **entries;
  tk_el_t *arrowtext;
  void *actionfunc_arrowtext;
} ud_boolean_t;
