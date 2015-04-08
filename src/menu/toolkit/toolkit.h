#pragma once
#include "control_data.h"
#include "internal.h"

// naming convention:
// init:   should be one instance only
// create: many instances

tk_t *tk_init(sty_t *fsty, sfx_t *sfx, const char *title);
void tk_init_gta2_background(tk_t *tk, const char *name);

tk_screen_t *tk_screen_create(tk_t *tk, void *ui_data,
                              tk_screen_layout_t layout, tk_screen_t *back,
                              void *event_func);

void tk_frame(tk_t *tk, SDL_Event *event);

void tk_screen_cleanup(tk_screen_t *screen);
void tk_cleanup(tk_t *tk);

void tk_screen_setbg(tk_t *tk, tk_screen_t *screen, const char *full,
                     const char *left, const char *right);

void tk_control_setbg(tk_t *tk, tk_control_t *ctrl, const char *full,
                      const char *left, const char *right);

/*
        Always use this macro for adding controls!

        Parameters:
                tk_screen_t* SCREEN,
                tk_control_t* UNINITIALIZED_CTRL,
                char* TITLE,
                char TYPE,
                        see control_data.h
                (c code) DATACODE,
                        fill data here (see example) or NULL

        Example:
                tk_control_t* rainbow_toggle;
                tk_control_add(screen, rainbow_toggle, TK_BOOLEAN,
                        "Enable Awesome Rainbows?",
                        data->value = true;
                );
*/
#define tk_control_add(SCREEN, UNINITIALIZED_CTRL, TITLE, TYPE, DATACODE)      \
  {                                                                            \
    TYPE##_DATA_t *data = malloc(sizeof(TYPE##_DATA_t));                       \
    memset(data, 0, sizeof(TYPE##_DATA_t));                                    \
    DATACODE;                                                                  \
    UNINITIALIZED_CTRL =                                                       \
        tk_control_add_internal(SCREEN, TITLE, TYPE, (void *)data);            \
  }

tk_control_t *tk_control_add_internal(tk_screen_t *screen, const char *title,
                                      char type, void *data);

// TODO:
// (load PNGs which have a transparent space at the position
//  of the GTA2 logo and create a texture with both images
//  combined)
// void tk_init_custom_backgrounds(tk_t* tk, const char* name);
