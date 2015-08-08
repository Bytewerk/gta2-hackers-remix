#include "toolkit.h"
#include "../../common/headers/tk_actions.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>

tk_t *tk_init(gxt_t *gxt, sty_t *fsty, sfx_t *sfx, bg_t *bg, char *pref_path,
              char *title) {
  tk_t *tk = calloc(1, sizeof(tk_t));
  tk->sfx = sfx;
  tk->fsty = fsty;
  tk->bg = bg;
  tk->gxt = gxt;
  tk->pref_path = pref_path;
  if (getenv("WINEPREFIX"))
    tk->wine = true;

  // currently, just use the fullscreen display (fake fullscreen at
  // desktop resultion!)
  // TODO:
  //	- hide windows taskbar (in 'meta' component, while init)
  //	- provide an option: run the mod in windowed mode
  //	- provide as option: change display number
  if (SDL_GetDesktopDisplayMode(0, &(tk->mode)) != 0)
    exit(printf("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError()));

  printf("desktop resolution: %ix%i\n", tk->mode.w, tk->mode.h);

  if (!tk->wine) {
    // FIXME: in native windows, load the resolution from a config
    tk->mode.w = 640;
    tk->mode.h = 480;
  }

  // create the SDL window
  tk->window = SDL_CreateWindow(title, 0, 0, tk->mode.w, tk->mode.h,
                                SDL_WINDOW_BORDERLESS);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);

  // set the virtual resolution to 640x480
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(tk->renderer, 640, 480);

  // set the window icon
  SDL_Surface *icon = IMG_Load("data/g2hr.png");
  SDL_SetWindowIcon(tk->window, icon);
  SDL_FreeSurface(icon);

  return tk;
}

// event is NULL when if the timelimit for the next
// frame has been reached
void tk_frame(tk_t *tk, SDL_Event *event) {
  tk_action_from_sdl_event(tk, event);

  // on screen change: force redraw and fire action
  if (tk->screen_last_frame != tk->screen_active) {
    tk->redraw_needed = 1;
    tk->screen_last_frame = tk->screen_active;
    tk_actions_recursive(tk, &(tk->screen_active->el),
                         tk->screen_active->el_selected,
                         TK_ACTION_BEFORE_FIRST_SCREEN_FRAME, SDLK_UNKNOWN);
  }

  if (!tk->disable_draw && tk->redraw_needed) {
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
