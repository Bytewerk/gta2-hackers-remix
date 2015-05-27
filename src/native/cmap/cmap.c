#include "cmap.h"
#include "../../common/cfg/cfg.h"
#include "../../common/headers/common.h"
#include "../../common/io/io.h"
#include <stdio.h>
#include <string.h>

// http://wiki.libsdl.org/SDL_GameControllerGetAxis
#define AXIS_OFFSET_MAX 32767
#define str(x) #x
#define DOCU "\n(more info: http://git.io/g2hr-mappings )\n"

#define CONVERT_BUTTON(BUTTON, CMAP_STR, SDL_SUFFIX)                           \
  {                                                                            \
    if (BUTTON == SDL_CONTROLLER_BUTTON_INVALID &&                             \
        !strcmp(str(SDL_SUFFIX), CMAP_STR))                                    \
      BUTTON = SDL_CONTROLLER_BUTTON_##SDL_SUFFIX;                             \
  }

#define CONVERT_STICK(AXIS, CMAP_STR, SDL_SUFFIX, IS_POSITIVE,                 \
                      STICKNAME /*"LEFT", "RIGHT" */, DIRECTION)               \
  {                                                                            \
    if (AXIS == SDL_CONTROLLER_AXIS_INVALID &&                                 \
        !strcmp(STICKNAME "STICK_" DIRECTION, CMAP_STR)) {                     \
      axis_is_positive = IS_POSITIVE;                                          \
      axis = SDL_CONTROLLER_AXIS_##SDL_SUFFIX;                                 \
    }                                                                          \
  }
#define STATE_AXIS                                                             \
  (axis_is_positive ? state->axis_positive : state->axis_negative)

#define CMAP_SDL_KEY_BUFFER_SIZE 100

void cmap_map_action(cmap_t *cmap, cfg_t *cfg, char *str_action,
                     cmap_action_t cmap_action, char is_required) {
  // transform the str_action in the form of "WALKING_ENTER_CAR"
  // into the form we actually need to find the config value:
  // walking/enter-car
  int len = strlen(str_action);
  char cfg_key[strlen(str_action) + 1];
  char seen_underscore = 0;
  for (int i = 0; i < len; i++) {
    char c = str_action[i];
    if (c == '_') {
      if (!seen_underscore) {
        c = '/';
        seen_underscore = 1;
      } else
        c = '-';
    } else
      c = c - 'A' + 'a';

    cfg_key[i] = c;
  }
  cfg_key[len] = '\0';

  // split the value, so one action can be assigned to multiple keys
  cfg_split_t *split = cfg_split_value(cfg, cfg_key, ',');
  if (!split) {
    if (!is_required)
      return;
    exit(printf("ERROR: couldn't find required value for '%s'"
                " (section/key format, case sensitive!). Please add it to"
                " the config and try again!" DOCU,
                cfg_key));
  }

  // iterate over all keys and save them to the struct
  cmap_state_t *state = (cfg_key[0] == 'w') ? &cmap->walking : &cmap->driving;
  for (int i = 0; i < split->count; i++) {
    SDL_GameControllerButton button = SDL_CONTROLLER_BUTTON_INVALID;
    SDL_GameControllerAxis axis = SDL_CONTROLLER_AXIS_INVALID;
    char axis_is_positive = 1;
    char *cmap_value = split->values[i];

    // BUTTONS (there's also SDL_GameControllerGetButtonFromString,
    // but 'dpup' isn't as readable as DPAD_UP)
    CONVERT_BUTTON(button, cmap_value, A);
    CONVERT_BUTTON(button, cmap_value, B);
    CONVERT_BUTTON(button, cmap_value, X);
    CONVERT_BUTTON(button, cmap_value, Y);
    CONVERT_BUTTON(button, cmap_value, LEFTSTICK);
    CONVERT_BUTTON(button, cmap_value, RIGHTSTICK);
    CONVERT_BUTTON(button, cmap_value, LEFTSHOULDER);
    CONVERT_BUTTON(button, cmap_value, RIGHTSHOULDER);
    CONVERT_BUTTON(button, cmap_value, DPAD_UP);
    CONVERT_BUTTON(button, cmap_value, DPAD_DOWN);
    CONVERT_BUTTON(button, cmap_value, DPAD_LEFT);
    CONVERT_BUTTON(button, cmap_value, DPAD_RIGHT);

    // AXES
    CONVERT_STICK(axis, cmap_value, LEFTY, 0, "LEFT", "UP");
    CONVERT_STICK(axis, cmap_value, LEFTY, 1, "LEFT", "DOWN");
    CONVERT_STICK(axis, cmap_value, LEFTX, 0, "LEFT", "LEFT");
    CONVERT_STICK(axis, cmap_value, LEFTX, 1, "LEFT", "RIGHT");
    CONVERT_STICK(axis, cmap_value, RIGHTY, 0, "RIGHT", "UP");
    CONVERT_STICK(axis, cmap_value, RIGHTY, 1, "RIGHT", "DOWN");
    CONVERT_STICK(axis, cmap_value, RIGHTX, 0, "RIGHT", "RIGHT");
    CONVERT_STICK(axis, cmap_value, RIGHTX, 1, "RIGHT", "RIGHT");

    // TRIGGERS (special case of axis)
    // (Yes, the names are inconsistent with SDL2. However, they
    // are consistent with the other axes, first the position on
    // the game pad and then the name)
    if (axis == SDL_CONTROLLER_AXIS_INVALID) {
      if (!strcmp(cmap_value, "LEFTTRIGGER"))
        axis = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
      else if (!strcmp(cmap_value, "RIGHTTRIGGER"))
        axis = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    }

    // verify if the value could be parsed
    if (button == SDL_CONTROLLER_BUTTON_INVALID &&
        axis == SDL_CONTROLLER_AXIS_INVALID)
      exit(printf("ERROR: invalid value '%s' (in %s)!" DOCU, cmap_value,
                  cfg_key));

    // actually map the buttons, if they have not been used in this
    // state (walking/driving) yet.
    char failed = 0;
    if (button != SDL_CONTROLLER_BUTTON_INVALID) {
      if (state->buttons[button])
        failed = 1;
      else
        state->buttons[button] = cmap_action;
    } else {
      if (STATE_AXIS[axis])
        failed = 1;
      else
        STATE_AXIS[axis] = cmap_action;
    }

    if (failed)
      exit(printf("ERROR: '%s' has been mapped twice in the '%s' section of"
                  " this config!" DOCU,
                  cmap_value, (cfg_key[0] == 'w') ? "walking" : "driving"));
  }
  cfg_split_cleanup(split);

  // TODO: remove key from cfg, so we can verify later that there
  // are no invalid keys
}
#undef CONVERT_BUTTON
#undef CONVERT_STICK
#undef STATE_AXIS

double cmap_sscanf_double_helper(char *str, char *cfg_key, int i) {
  double d = 0;
  if (!sscanf(str, "%lf", &d))
    exit(printf("ERROR: invalid value '%s' in '%s' (item %i)."
                " Expected a double here (eg. 0.5)." DOCU,
                str, cfg_key, i));
  return d;
}

#define SPLITVAL2FLOAT(NUM)                                                    \
  cmap_sscanf_double_helper(split->values[NUM], cfg_key, NUM)

void cmap_map_deadzone_stick(cfg_t *cfg, char *cfg_key, cmap_deadzone_t *d) {
  if (d->left)
    exit(printf("ERROR: deadzone has been defined"
                " twice in the same section: %s" DOCU,
                cfg_key));

  // default is always 0.5
  d->left = d->up = d->right = d->down = 0.5 * AXIS_OFFSET_MAX;

  // read the space separated values
  cfg_split_t *split = cfg_split_value(cfg, cfg_key, ' ');
  if (!split)
    return;

  // factors (left, up, right, down), parsed symmetrically just like
  // in CSS (eg. padding: ...)
  double f[] = {0, 0, 0, 0};
  switch (split->count) {
  case 1:
    f[0] = f[1] = f[2] = f[3] = SPLITVAL2FLOAT(0);
    break;
  case 2:
    f[0] = f[2] = SPLITVAL2FLOAT(0); // left, right
    f[1] = f[3] = SPLITVAL2FLOAT(1); // up, down
    break;
  case 3:
    f[0] = SPLITVAL2FLOAT(0);        // left
    f[1] = f[3] = SPLITVAL2FLOAT(1); // up, down
    f[2] = SPLITVAL2FLOAT(2);        // right
    break;
  case 4:
    f[0] = SPLITVAL2FLOAT(0);
    f[1] = SPLITVAL2FLOAT(1);
    f[2] = SPLITVAL2FLOAT(2);
    f[3] = SPLITVAL2FLOAT(3);
    break;
  default:
    exit(printf("ERROR: invalid value count for %s! Should"
                " be 1...4" DOCU,
                cfg_key));
  }
  cfg_split_cleanup(split);

  d->left = f[0] * AXIS_OFFSET_MAX;
  d->up = f[1] * AXIS_OFFSET_MAX;
  d->right = f[2] * AXIS_OFFSET_MAX;
  d->down = f[3] * AXIS_OFFSET_MAX;

  // TODO: remove key from cfg, so we can verify later that there
  // are no invalid keys
}

#define MAPPING(IS_REQUIRED, ACTION)                                           \
  cmap_map_action(cmap, cfg, str(ACTION), G2HR_CMAP_##ACTION, IS_REQUIRED)

#define DEADZONE_STICK(NAME)                                                   \
  cmap_map_deadzone_stick(cfg, "walking/deadzone-" str(NAME),                  \
                          &(cmap->walking.dead_##NAME));                       \
  cmap_map_deadzone_stick(cfg, "driving/deadzone-" str(NAME),                  \
                          &(cmap->driving.dead_##NAME))

void cmap_load_file(char *filename, void *userdata) {
  cmap_init_t *list = (cmap_init_t *)userdata;
  cmap_t *cmap = calloc(1, sizeof(cmap_t));

  cfg_t *cfg = cfg_load(filename, 0);

  MAPPING(1, WALKING_FORWARD);
  MAPPING(1, WALKING_BACKWARD);
  MAPPING(1, WALKING_LEFT);
  MAPPING(1, WALKING_RIGHT);
  MAPPING(1, WALKING_ATTACK);
  MAPPING(1, WALKING_ENTER_CAR);
  MAPPING(1, WALKING_JUMP);
  MAPPING(1, WALKING_WEAPON_PREV);
  MAPPING(1, WALKING_WEAPON_NEXT);

  // all driving keys are optional. fallback is: using the same
  // controls as for walking (not implemented that way yet though)
  MAPPING(0, DRIVING_FORWARD);
  MAPPING(0, DRIVING_BACKWARD);
  MAPPING(0, DRIVING_LEFT);
  MAPPING(0, DRIVING_RIGHT);
  MAPPING(0, DRIVING_ATTACK);
  MAPPING(0, DRIVING_EXIT_CAR);
  MAPPING(0, DRIVING_HANDBRAKE);
  MAPPING(0, DRIVING_WEAPON_PREV);
  MAPPING(0, DRIVING_WEAPON_NEXT);

  // add all deadzones (fallback is always 0.5)
  DEADZONE_STICK(leftstick);
  DEADZONE_STICK(rightstick);

  cfg_cleanup(cfg);

  // add it to the list
  if (list->first)
    list->last->next = cmap;
  else
    list->first = cmap;
  list->last = cmap;
}

#undef MAPPING
#undef DEADZONE_STICK

cmap_t *cmap_init() {
  cmap_init_t *userdata = calloc(1, sizeof(cmap_init_t));

  io_iterate_over_files_in_folder("data/controller-mappings", "ini",
                                  cmap_load_file, (void *)userdata, 1);

  cmap_t *first = userdata->first;
  free(userdata);
  return first;
}

#define MAPPING(ACTION, BITMASK)                                               \
  case G2HR_CMAP_##ACTION:                                                     \
    return GTA2_CTRL_##BITMASK

uint16_t cmap_action_to_movement_bitmask(cmap_action_t action) {
  switch (action) {
    MAPPING(NONE, NOT_MOVING);

    MAPPING(WALKING_FORWARD, FORWARD);
    MAPPING(WALKING_BACKWARD, BACKWARD);
    MAPPING(WALKING_LEFT, LEFT);
    MAPPING(WALKING_RIGHT, RIGHT);
    MAPPING(WALKING_ATTACK, ATTACK);
    MAPPING(WALKING_ENTER_CAR, ENTER_EXIT_CAR);
    MAPPING(WALKING_JUMP, JUMP_HANDBRAKE);
    MAPPING(WALKING_WEAPON_PREV, WEAPON_PREV);
    MAPPING(WALKING_WEAPON_NEXT, WEAPON_NEXT);

    MAPPING(DRIVING_FORWARD, FORWARD);
    MAPPING(DRIVING_BACKWARD, BACKWARD);
    MAPPING(DRIVING_LEFT, LEFT);
    MAPPING(DRIVING_RIGHT, RIGHT);
    MAPPING(DRIVING_ATTACK, ATTACK);
    MAPPING(DRIVING_EXIT_CAR, ENTER_EXIT_CAR);
    MAPPING(DRIVING_HANDBRAKE, JUMP_HANDBRAKE);
    MAPPING(DRIVING_WEAPON_PREV, WEAPON_PREV);
    MAPPING(DRIVING_WEAPON_NEXT, WEAPON_NEXT);
  }
  return GTA2_CTRL_NOT_MOVING;
}
#undef MAPPING

void cmap_cleanup(cmap_t *cmap) {
  while (cmap) {
    cmap_t *old = cmap;
    cmap = cmap->next;
    free(old);
  }
}
