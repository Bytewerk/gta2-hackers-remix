#include "pads.h"
#include <stdio.h>
#include <string.h>

void pads_detach_all(pads_t *pads) {
  pad_controller_t *current = pads->first;
  while (current) {
    pad_controller_t *old = current;
    current = old->next;

    // first close haptic before device!
    if (old->haptic)
      SDL_HapticClose(old->haptic);
    if (SDL_GameControllerGetAttached(old->controller))
      SDL_GameControllerClose(old->controller);

    free(old);
  }
  pads->count = 0;
  pads->first = NULL;
}

void pads_attach_all(pads_t *pads, bool v /*verbose*/) {
  if (pads->count)
    pads_detach_all(pads);
  pad_controller_t *last = NULL;

  int joy_count = SDL_NumJoysticks();
  if (v)
    printf(" ==== scanning for controllers ==== \n");
  for (int i = 0; i < joy_count; i++) {
    if (v)
      printf("# %i: %s\n", i, SDL_JoystickNameForIndex(i));

    if (!SDL_IsGameController(i)) {
      if (v)
        printf("not a SDL game controller, skipping...\n");
      continue;
    }

    SDL_GameController *controller = SDL_GameControllerOpen(i);
    if (!controller) {
      if (v)
        printf("failed to attach: %s\n", SDL_GetError());
      continue;
    }
    if (v)
      printf("name: %s\n", SDL_GameControllerName(controller));

    pad_controller_t *new = calloc(1, sizeof(pad_controller_t));
    new->controller = controller;
    new->joystick = SDL_GameControllerGetJoystick(controller);
    new->joystick_id = SDL_JoystickInstanceID(new->joystick);
    new->haptic = SDL_HapticOpenFromJoystick(new->joystick);

    if (last)
      last->next = new;
    else
      pads->first = new;
    last = new;
    pads->count++;

    if (v)
      printf("attached!\n");
  }
  if (v)
    printf(" ==== scanning done! ==== \n");
}

pads_t *pads_init(bool verbose) {
  pads_t *pads = calloc(1, sizeof(pads_t));
  pads_attach_all(pads, verbose);
  return pads;
}

void pads_frame(pads_t *pads, SDL_Event *event, bool verbose) {
  if (event->type == SDL_CONTROLLERDEVICEADDED ||
      event->type == SDL_CONTROLLERDEVICEREMOVED ||
      event->type == SDL_CONTROLLERDEVICEREMAPPED) {
    pads_attach_all(pads, verbose);
  }
}

void pads_cleanup(pads_t *pads) {
  pads_detach_all(pads);
  free(pads);
}
