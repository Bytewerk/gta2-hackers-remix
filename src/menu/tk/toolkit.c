#include "toolkit.h"
#include "actions.h"

tk_t *tk_init(sty_t *fsty, sfx_t *sfx, bg_t *bg, char *title) {
  tk_t *tk = calloc(1, sizeof(tk_t));
  tk->sfx = sfx;
  tk->fsty = fsty;
  tk->bg = bg;

  // create the SDL window
  tk->window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);

  // set the virtual resolution to 640x480
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(tk->renderer, 640, 480);

  return tk;
}

void tk_frame(tk_t *tk, SDL_Event *event) {
  tk_action(tk, event);
  SDL_RenderClear(tk->renderer);
  tk_screen_draw(tk);
  SDL_RenderPresent(tk->renderer);
}

void tk_cleanup(tk_t *tk) {

  SDL_DestroyRenderer(tk->renderer);
  SDL_DestroyWindow(tk->window);
  free(tk);
}
