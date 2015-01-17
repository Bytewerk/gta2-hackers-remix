#include "toolkit.h"
#include <stdlib.h>

void tk_screen_setbg(tk_t *tk, tk_screen_t *screen, const char *full,
                     const char *left, const char *right) {
  if (screen->bg)
    free(screen->bg);
  screen->bg = tk_create_background(tk, full, left, right);
}

tk_screen_t *tk_screen_create(tk_t *tk, void *ui_data, tk_screen_t *back,
                              void *event_func) {
  tk_screen_t *screen = malloc(sizeof(tk_screen_t));
  screen->back = back;
  screen->first_control = NULL;
  screen->selected_control = NULL;
  screen->bg = NULL;
  screen->ui_data = ui_data;
  screen->event_func = event_func;

  return screen;
}

// prefer element background over screen background,
// only draw the background if one is actually set.
#define DRAWBG(WHERE, XPOS, YPOS, WIDTH, HEIGHT)                               \
  {                                                                            \
    if (bg->WHERE) {                                                           \
      SDL_Rect dstrect = {XPOS, YPOS, WIDTH, HEIGHT};                          \
      SDL_RenderCopy(tk->renderer, bg->WHERE->texture, NULL, &dstrect);        \
    };                                                                         \
  }

void tk_screen_draw(tk_t *tk) {
  tk_screen_t *screen = tk->screen;
  tk_control_t *control = screen->selected_control;
  tk_background_t *bg = (control && control->bg) ? control->bg : screen->bg;

  if (bg) {
    DRAWBG(full, 000, 000, 640, 480);
    DRAWBG(left, 000, 000, 320, 480);
    DRAWBG(right, 320, 000, 320, 480);
  }

  printf("\n");
  tk_control_t *ctrl = screen->first_control;
  while (ctrl) {
    printf("[%c] %s\n", (ctrl == screen->selected_control) ? 'x' : ' ',
           ctrl->title);
    ctrl = ctrl->next;
  }
}

#undef DRAWBG

void tk_screen_cleanup(tk_screen_t *screen) {
  if (screen->bg)
    free(screen->bg);

  // clean up all controls (todo: verify with valgrind!)
  tk_control_t *ctrl = screen->first_control;
  while (ctrl)
    ctrl = tk_control_cleanup(ctrl);
}

void tk_screen_back(tk_t *tk) {
  // TODO: clean up everything!
  if (!tk->screen || !tk->screen->back)
    exit(0);

  tk->screen = tk->screen->back;
}
