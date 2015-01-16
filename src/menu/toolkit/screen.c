#include "toolkit.h"

tk_screen_t *tk_screen_create(tk_t *tk, tk_screen_t *back, const char *full,
                              const char *left, const char *right) {
  tk_screen_t *screen = malloc(sizeof(tk_screen_t));
  screen->first_control = NULL;
  screen->selected_control = NULL;

  if (full || left || right) {
    tk_background_t *bg = malloc(sizeof(tk_background_t));
    bg->full = full ? tk_texture_get(tk, full) : NULL;
    bg->left = left ? tk_texture_get(tk, left) : NULL;
    bg->right = right ? tk_texture_get(tk, right) : NULL;
    screen->bg = bg;
  } else
    screen->bg = NULL;

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

  // TODO: draw controls!
}

#undef DRAWBG

void tk_screen_cleanup(tk_screen_t *screen) {
  if (screen->bg)
    free(screen->bg);
  // TODO: cleanup all controls! (create controls.c/.h ?)
}
