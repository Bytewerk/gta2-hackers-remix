#include "pads.h"
#include <stdio.h>
#include <string.h>

/*
        FIXME: This detection is pretty crappy, but it's hard to do
        when the GUID is useless for Xbox360 controllers (they all have
        the same one!) and the joystick_id changes as the OS decides...
*/
pad_controller_t *pads_is_joystick_attached(pads_t *pads,
                                            SDL_JoystickID joystick_id) {
  pad_controller_t *pad = pads->first;
  while (pad) {
    if (joystick_id == pad->joystick_id) {
      return pad;
    }
    pad = pad->next;
  }
  return NULL;
}

void pads_add(pads_t *pads, char verbose) {
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

    pad_controller_t *pad = pads_is_joystick_attached(pads, i);
    if (pad && !pad->disconnected) {
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

    printf("[native] %sattached game controller: %s (%s)\n", (pad ? "re-" : ""),
           guid, SDL_GameControllerName(controller));

    // controller is open, re-fill the old struct
    // or create a new one
    char attach_to_list = (pad == NULL);
    if (pad) {
      pad->disconnected = 0;
      free(guid);
    } else {
      pad = calloc(1, sizeof(pad_controller_t));
      pad->guid = guid;
    }

    pad->controller = controller;
    pad->joystick_id =
        SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

    // pad->haptic =
    pads->count++;

    // attach to the list
    if (attach_to_list) {
      if (last)
        last->next = pad;
      else
        pads->first = pad;
      last = pad;
    }
  }
  if (verbose)
    printf("scan finished! count of attached controllers: %i\n", pads->count);
}

void pads_rm(pads_t *pads, char verbose) {
  pad_controller_t *current = pads->first;
  while (current) {
    if (!current->disconnected &&
        !SDL_GameControllerGetAttached(current->controller)) {
      SDL_GameControllerClose(current->controller);
      current->disconnected = 1;
      printf("[native] game controller disconnected: %s\n", current->guid);
      pads->count--;
    }
    current = current->next;
  }
}

pads_t *pads_init(char verbose) {
  pads_t *pads = calloc(1, sizeof(pads_t));
  pads_add(pads, verbose);
  return pads;
}

void pads_frame(pads_t *pads, SDL_Event *event, char verbose) {
  switch (event->type) {
  case SDL_CONTROLLERDEVICEADDED: {
    pads_add(pads, verbose);
    break;
  }
  case SDL_CONTROLLERDEVICEREMOVED: {
    pads_rm(pads, verbose);
    break;
  }
  case SDL_CONTROLLERDEVICEREMAPPED: {
    printf("[native] SDL2 says, that some game controller device has been "
           "re-mapped. Not really sure what that means, so not doing anything. "
           "If you think this is a bug and GTA2: Hacker's Remix should so "
           "something else here, please report a bug at: "
           "http://git.io/g2hr-bugs");
    break;
  }
  }
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
