#include "menu.h"
#include <stdlib.h>

menu_t *menu_init() {
  menu_t *menu = malloc(sizeof(menu_t));
  menu->controller_count = 0;
  menu->screen = MAIN;

  return menu;
}

void menu_cleanup(menu_t *menu) { free(menu); }
