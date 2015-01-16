#include "toolkit.h"
#include "../sty_loader/sty_parser.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// call SDL_init before!
tk_t *tk_init(const char *title) {
  tk_t *tk = malloc(sizeof(tk_t));
  tk->screen = NULL;
  tk->textures = NULL;
  tk->fstyle = NULL; // sty_parser("data/fstyle.sty");

  // create the SDL window
  tk->window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN);
  if (!tk->window)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));

  tk->renderer = SDL_CreateRenderer(tk->window, -1, 0);
  SDL_SetRenderDrawColor(tk->renderer, 0, 0, 0, 0); // black

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
  exit(1);
}

void tk_cleanup(tk_t *tk) {
  // sty_cleanup(tk->fstyle);

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

  // fade to the next background, if it has changed

  if (tk->screen) {
    tk_screen_draw(tk);

    if (event->type == SDL_KEYDOWN) {
      // go to next control (TODO: create an extra
      // function for this)

      tk_control_prev(tk->screen);
    }
  }

  SDL_RenderPresent(tk->renderer);
}
