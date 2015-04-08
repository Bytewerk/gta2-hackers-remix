#include "toolkit.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// call SDL_init before!
tk_t *tk_init(sty_t *fsty, sfx_t *sfx, const char *title) {
  tk_t *tk = malloc(sizeof(tk_t));
  tk->fsty = fsty;
  tk->sfx = sfx;
  tk->screen = NULL;
  tk->textures = NULL;
  tk->quit = 0;

  // create the SDL window
  tk->window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);
  SDL_SetRenderDrawColor(tk->renderer, 0, 0, 0, 0); // black

  // set the virtual resolution to 640x480
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(tk->renderer, 640, 480);

  return tk;
}

tk_texture_t *tk_texture_get(tk_t *tk, const char *name) {
  tk_texture_t *listpos = tk->textures;
  while (listpos) {
    if (!strcmp(listpos->name, name))
      return listpos;
    listpos = listpos->next;
  }

  printf("ERROR: tried to access not loaded texture: %s\n", name);
  tk->quit = 1;
  return NULL;
}

void tk_cleanup(tk_t *tk) {
  // free all textures (TODO: verify with valgrind!)
  tk_texture_t *listpos = tk->textures;
  while (listpos) {
    tk_texture_t *delme = listpos;
    listpos = delme->next;
    SDL_DestroyTexture(delme->texture);
    free(delme);
  }

  if (tk->screen)
    tk_screen_cleanup(tk->screen);
  SDL_DestroyRenderer(tk->renderer);
  SDL_DestroyWindow(tk->window);
  free(tk);
}

void tk_frame(tk_t *tk, SDL_Event *event) {
  SDL_RenderClear(tk->renderer);

  // switch to the next background, if it has changed
  // TODO: also handle mouse input!
  // TODO: add backspace => esc
  // TODO: rename tk_control* functions here to tk_action_* ?
  // TODO: rename _enter to _onclick

  if (tk->screen) {
    tk_screen_draw(tk);

    if (event->type == SDL_KEYDOWN) {
      SDL_Keycode key = event->key.keysym.sym;
      if (key == SDLK_UP)
        tk_control_up(tk->screen);
      if (key == SDLK_DOWN)
        tk_control_down(tk->screen);
      if (key == SDLK_ESCAPE || key == SDLK_BACKSPACE)
        tk_screen_back(tk);
      if (key == SDLK_RETURN)
        tk_control_enter(tk);
    }
  }

  SDL_RenderPresent(tk->renderer);
}
