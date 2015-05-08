#include "toolkit.h"
#include "../../common/tk_actions.h"

tk_t *tk_init(gxt_t *gxt, sty_t *fsty, sfx_t *sfx, bg_t *bg, char *title) {
  tk_t *tk = calloc(1, sizeof(tk_t));
  tk->sfx = sfx;
  tk->fsty = fsty;
  tk->bg = bg;
  tk->gxt = gxt;

  // currently, just use the fullscreen display (fake fullscreen at
  // desktop resultion!)
  // TODO:
  //	- hide windows taskbar (in 'meta' component, while init)
  //	- provide an option: run the mod in windowed mode
  //	- provide as option: change display number
  SDL_DisplayMode mode;
  if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
    exit(printf("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError()));

  printf("desktop resolution: %ix%i\n", mode.w, mode.h);

  // DEBUG (for taking screenshots)
  mode.w = 640;
  mode.h = 480;

  // create the SDL window
  tk->window = SDL_CreateWindow(title, 0, 0, mode.w, mode.h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);

  // set the virtual resolution to 640x480
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(tk->renderer, 640, 480);

  return tk;
}

// event is NULL when if the timelimit for the next
// frame has been reached
void tk_frame(tk_t *tk, SDL_Event *event) {
  tk_action(tk, event);

  // on screen change: force redraw and fire action
  if (tk->screen_last_frame != tk->screen_active) {
    tk->redraw_needed = 1;
    tk->screen_last_frame = tk->screen_active;
    tk_actions_recursive(tk, &(tk->screen_active->el),
                         tk->screen_active->el_selected,
                         TK_ACTION_BEFORE_FIRST_SCREEN_FRAME, SDLK_UNKNOWN);
  }

  if (tk->redraw_needed) {
    tk->redraw_needed = 0;
    SDL_RenderClear(tk->renderer);
    tk_screen_draw(tk);
    SDL_RenderPresent(tk->renderer);
  }
}

void tk_cleanup(tk_t *tk) {
  tk_el_clean(tk, &(tk->screen_first->el));

  SDL_DestroyRenderer(tk->renderer);
  SDL_DestroyWindow(tk->window);
  free(tk);
}
