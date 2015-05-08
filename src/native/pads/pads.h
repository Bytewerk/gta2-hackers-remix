#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

#define G2HR_PADS_GUID_LENGTH 100

typedef struct pad_controller_t {
  struct pad_controller_t *next;

  SDL_GameController *controller;
  char *guid;
  // SDL_Haptic* haptic;

} pad_controller_t;

typedef struct {
  pad_controller_t *first;
  int count;
} pads_t;

pads_t *pads_init(char verbose);

void pads_frame(pads_t *pads, SDL_Event *event);

void pads_cleanup(pads_t *pads);
