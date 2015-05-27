#pragma once
#include "cmap_gta2_controls.h"
#include <SDL2/SDL.h>
#include <stdint.h>

/*
        we abstract the "gta2_controls.h" (as stored in memory of the GTA2
        instances) even further, so we can add different controls for
        walking and driving, as well as camera controls!
*/
typedef enum {
  G2HR_CMAP_NONE,

  G2HR_CMAP_WALKING_FORWARD,
  G2HR_CMAP_WALKING_BACKWARD,
  G2HR_CMAP_WALKING_LEFT,
  G2HR_CMAP_WALKING_RIGHT,
  G2HR_CMAP_WALKING_ATTACK,
  G2HR_CMAP_WALKING_ENTER_CAR,
  G2HR_CMAP_WALKING_JUMP,
  G2HR_CMAP_WALKING_WEAPON_PREV,
  G2HR_CMAP_WALKING_WEAPON_NEXT,

  G2HR_CMAP_DRIVING_FORWARD,
  G2HR_CMAP_DRIVING_BACKWARD,
  G2HR_CMAP_DRIVING_LEFT,
  G2HR_CMAP_DRIVING_RIGHT,
  G2HR_CMAP_DRIVING_ATTACK,
  G2HR_CMAP_DRIVING_EXIT_CAR,
  G2HR_CMAP_DRIVING_HANDBRAKE,
  G2HR_CMAP_DRIVING_WEAPON_PREV,
  G2HR_CMAP_DRIVING_WEAPON_NEXT,

  // TODO: SPECIAL1, SPECIAL2, CAMERA

} cmap_action_t;

typedef struct {
  uint16_t left;
  uint16_t up;
  uint16_t right;
  uint16_t down;
} cmap_deadzone_t;

typedef struct {
  cmap_action_t buttons[SDL_CONTROLLER_BUTTON_MAX];
  cmap_action_t axis_positive[SDL_CONTROLLER_AXIS_MAX];
  cmap_action_t axis_negative[SDL_CONTROLLER_AXIS_MAX];

  cmap_deadzone_t dead_leftstick;
  cmap_deadzone_t dead_rightstick;

  // not supported - if you need these, file a feature request
  // at http://git.io/g2hr-bugs
  //
  // uint16_t dead_lefttrigger;
  // uint16_t dead_righttrigger;
} cmap_state_t;

typedef struct cmap_t {
  // currently unused - this will be for multiple configs!
  struct cmap_t *next;

  char *description;
  char *author;
  char *version;

  cmap_state_t walking;
  cmap_state_t driving;

} cmap_t;

// internal struct that gets used while iterating over all configs
typedef struct {
  cmap_t *first;
  cmap_t *last;

} cmap_init_t;

cmap_t *cmap_init();

uint16_t cmap_action_to_movement_bitmask(cmap_action_t action);

void cmap_cleanup(cmap_t *cmap);
