#pragma once
#include "../bg/background.h"
#include "el_flags.h"
#include <stdint.h>

typedef enum {
  // basic
  STACK,
  FLOW,

  // extended (keep these to a minimum, implement all others
  // as controls on top of those!)
  LABEL,
  SPRITE

} tk_el_type_t;

/*
        very basic element type, that allows recursion
        and setting the type.
        width, height gets calculated automatically for stacks and flows.
*/
typedef struct tk_el_t {
  // list head
  struct tk_el_t *next;
  struct tk_el_t *sub;
  struct tk_el_t *parent;
  tk_el_type_t type;

  // stack specific
  uint16_t scroll_top;

  // extended type specific
  char sprite_id;
  char *text;
  char font_id;
  char font_id_selected;

  // set these directly
  uint32_t argb_normal;   // 0x00000000: not used
  uint32_t argb_selected; // 0x00000000: not used
  uint32_t argb_disabled; // 0x00000000: not used
  void *actionfunc;       // (tk, el, el_selected, action, key)
  void *userdata;
  bg_mashup_t *bg_mashup;
  const char *bottom_text_high;
  const char *bottom_text_low;

  // set these with the macros in el_flags.h
  char flags;
  uint16_t width;
  uint16_t height;
  int16_t padding_left;
  int16_t padding_top;
  int16_t padding_right;
  int16_t padding_bottom;
} tk_el_t;

/*
        STACKS 'n' FLOWS - idea from the "Shoes" UI Framework

        These are implemented as macros, so they can be nested easily.
        Always define TK_PARENT and 'tk' before using them! Example:

                tk_t* tk = tk_init();
                tk_el_t* TK_PARENT = calloc(1,sizeof(tk_el_t));

                TK_STACK
                (
                        TK_FLOW
                        (
                                tk_label(TK_PARENT, "go");
                                tk_label(TK_PARENT, "with");
                                tk_label(TK_PARENT, "the");
                                tk_label(TK_PARENT, "flow");
                        )
                        tk_label(TK_PARENT, "with");
                        tk_label(TK_PARENT, "the");
                        tk_label(TK_PARENT, "stack");
                )

        Result:
                go with the flow
                with
                the
                stack
*/

#define TK_STACK(UICODE)                                                       \
  {                                                                            \
    tk_el_t *TK_PARENT_OLD = TK_PARENT;                                        \
    {                                                                          \
      TK_PARENT =                                                              \
          tk_el_attach(calloc(1, sizeof(tk_el_t)), TK_PARENT_OLD, STACK);      \
      UICODE;                                                                  \
      tk_el_geocalc(tk, TK_PARENT, 0, 0);                                      \
      TK_PARENT = TK_PARENT_OLD;                                               \
    }                                                                          \
  }
#define TK_FLOW(UICODE)                                                        \
  {                                                                            \
    tk_el_t *TK_PARENT_OLD = TK_PARENT;                                        \
    {                                                                          \
      TK_PARENT =                                                              \
          tk_el_attach(calloc(1, sizeof(tk_el_t)), TK_PARENT_OLD, FLOW);       \
      UICODE;                                                                  \
      tk_el_geocalc(tk, TK_PARENT, 0, 0);                                      \
      TK_PARENT = TK_PARENT_OLD;                                               \
    }                                                                          \
  }
