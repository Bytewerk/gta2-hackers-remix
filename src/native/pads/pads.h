#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

#define G2HR_PADS_GUID_LENGTH 100

typedef struct pad_controller_t {
  struct pad_controller_t *next;

  // pads, that were attached earlier and get disconnected after
  // that, do not get deleted. instead, when they get re-attached,
  // they have the same position in the list again.
  char disconnected;

  SDL_GameController *controller;

  // FIXME: the guid is the same for all xbox 360 controllers -.-
  // find another way to identify them / just try if we can connect?
  char *guid;
  // SDL_Haptic* haptic;

} pad_controller_t;

typedef struct {
  pad_controller_t *first;
  int count;
} pads_t;

pads_t *pads_init(char verbose);

void pads_frame(pads_t *pads, SDL_Event *event, char verbose);

void pads_cleanup(pads_t *pads);
