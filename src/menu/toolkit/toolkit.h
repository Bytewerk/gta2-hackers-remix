#pragma once
#include "control_data.h"
#include "internal.h"

// naming convention:
// init:   should be one instance only
// create: many instances

tk_t *tk_init(const char *title);
void tk_init_gta2_background(tk_t *tk, const char *name);

tk_screen_t *tk_screen_create(tk_t *tk, const char *full, const char *left,
                              const char *right);

void tk_frame(tk_t *tk);

void tk_screen_cleanup(tk_screen_t *screen);
void tk_cleanup(tk_t *tk);

// TODO:
// (load PNGs which have a transparent space at the position
//  of the GTA2 logo and create a texture with both images
//  combined)
// void tk_init_custom_backgrounds(tk_t* tk, const char* name);

// TODO:
// tk_element_t* tk_screen_add_control(tk_screen_t* screen,
//	char type, void* event_func, const char* title);

// {"1", "1_Play", "1_Options", "1_Quit"}
