#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define G2HR_PADS_GUID_LENGTH 100

typedef struct pad_controller_t {
  struct pad_controller_t *next;

  SDL_GameController *controller;
  SDL_Joystick *joystick;
  SDL_JoystickID joystick_id;
  SDL_Haptic *haptic;

} pad_controller_t;

typedef struct {
  pad_controller_t *first;
  int count;
} pads_t;

pads_t *pads_init(bool verbose);

void pads_frame(pads_t *pads, SDL_Event *event, bool verbose);

void pads_cleanup(pads_t *pads);
