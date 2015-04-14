#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

/*
        TODO:
                - add custom backgrounds
                - add draw function
*/

// single background texture (list)
typedef struct bg_t {
  struct bg_t *next;
  const char *name;
  SDL_Surface *surface;
} bg_t;

// one specific mashup of a one or more textures
typedef struct {
  bg_t *full;
  bg_t *left;
  bg_t *right;
  bg_t *custom;
} bg_mashup_t;

bg_t *bg_init(const char **tgas);

bg_mashup_t *bg_mashup(bg_t *bg, const char *full, const char *left,
                       const char *right, const char *custom);

void bg_cleanup(bg_t *bg);
