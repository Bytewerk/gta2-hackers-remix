#pragma once

typedef enum { MAIN, SPLITSCREEN_SETUP, SETTINGS, CREDITS } screen_t;

typedef struct {
  unsigned char controller_count;
  screen_t screen;

} menu_t;

menu_t *menu_init();
