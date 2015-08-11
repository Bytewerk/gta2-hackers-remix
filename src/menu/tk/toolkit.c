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

  // create the SDL window
  tk->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);

  // set the virtual resolution to 640x480
  SDL_RenderSetLogicalSize(tk->renderer, 640, 480);

  // set the window icon
  // FIXME: find a way (native windows api?) to set a 16x16 and 32x32
  // icon, right now we can only have one resolution for both!

  SDL_Surface *icon = IMG_Load("data/g2hr.png");
  SDL_SetWindowIcon(tk->window, icon);
  SDL_FreeSurface(icon);

  // TODO: maybe one day, a gta2-90s-style cursor could be set here,
  // but that only makes sense when the menu can be used with a mouse
  // (which is not the case now!)
  SDL_ShowCursor(SDL_DISABLE);

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
