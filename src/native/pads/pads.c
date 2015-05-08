#include "pads.h"
#include <stdio.h>
#include <string.h>

char pads_is_joystick_attached(pads_t *pads, char *guid) {
  pad_controller_t *pad = pads->first;
  while (pad) {
    if (!strcmp(guid, pad->guid))
      return 1;
    pad = pad->next;
  }
  return 0;
}

void pads_scan(pads_t *pads, char verbose) {
  int joy_count = SDL_NumJoysticks();

  pad_controller_t *last = pads->first;
  while (last)
    last = last->next;

  if (verbose)
    printf("======== DEBUG: SCANNING FOR CONTROLLERS ========\n");

  for (int i = 0; i < joy_count; i++) {
    if (verbose)
      printf("=== joystick %i: %s\n", i, SDL_JoystickNameForIndex(i));
    if (!SDL_IsGameController(i)) {
      if (verbose)
        printf("not a sdl game controller, skipping...\n");
      continue;
    }

    // generate the guid as string, so we can compare it
    char *guid = malloc(G2HR_PADS_GUID_LENGTH + 1);
    SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid,
                              G2HR_PADS_GUID_LENGTH);
    if (verbose)
      printf("guid: %s\n", guid);

    if (pads_is_joystick_attached(pads, guid)) {
      if (verbose)
        printf("this sdl game controller is already attached, skipping...\n");
      free(guid);
      continue;
    }

    SDL_GameController *controller = SDL_GameControllerOpen(i);
    if (!controller) {
      if (verbose)
        printf("this is a sdl game controller, but it couldn't be opened!");
      free(guid);
      continue;
    }

    if (verbose)
      printf("game controller name: %s\n", SDL_GameControllerName(controller));

    // controller is open, create the full struct
    pad_controller_t *pad = calloc(1, sizeof(pad_controller_t));
    pad->controller = controller;
    pad->guid = guid;
    // pad->haptic =

    // attach to the list
    pads->count++;
    if (last)
      last->next = pad;
    else
      pads->first = pad;
    last = pad;
  }
  if (verbose)
    printf("scan finished! count of attached controllers: %i\n", pads->count);
}

pads_t *pads_init(char verbose) {
  pads_t *pads = calloc(1, sizeof(pads_t));
  pads_scan(pads, verbose);
  return pads;
}

void pads_frame(pads_t *pads, SDL_Event *event) {
  // TODO - handle events: attached (scan),
  // detatched (close and drop from ist), remapped (?)
}

void pads_cleanup(pads_t *pads) {
  pad_controller_t *current = pads->first;
  while (current) {
    pad_controller_t *old = current;
    current = old->next;
    free(old->guid);
    SDL_GameControllerClose(old->controller);
    // close haptic
    free(old);
  }
  free(pads);
}
