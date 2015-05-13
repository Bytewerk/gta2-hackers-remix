#include "cmap.h"
#include "../../common/cfg/cfg.h"
#include "../../common/common.h"
#include <stdio.h>
#include <string.h>

#define str(x) #x

/*
        TODO: add move file loading from mmp to cfg and also use it here
        to load all ini files in that directory!

*/

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

#define CMAP_SDL_KEY_BUFFER_SIZE 100
#define DOCU                                                                   \
  "\n(more info:"                                                              \
  " http://git.io/g2hr-mappings )\n"

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
    CONVERT_BUTTON(button, cmap_value, BACK);
    CONVERT_BUTTON(button, cmap_value, START);
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
    if (axis == SDL_CONTROLLER_AXIS_INVALID) {
      if (!strcmp(cmap_value, "TRIGGERLEFT"))
        axis = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
      else if (!strcmp(cmap_value, "TRIGGERRIGHT"))
        axis = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    }

    // verify if the value could be parsed
    if (button == SDL_CONTROLLER_BUTTON_INVALID &&
        axis == SDL_CONTROLLER_AXIS_INVALID)
      exit(printf("ERROR: invalid value '%s' (in %s)!" DOCU, cmap_value,
                  cfg_key));

    // actually map the buttons, if they have not been used in this
    // state (walking/driving) yet.
    if (button != SDL_CONTROLLER_BUTTON_INVALID) {
      if (state->buttons[button])
        exit(printf("ERROR: the button '%s' has been mapped"
                    " twice in the '%s' section of this config!" DOCU,
                    cmap_value, (cfg_key[0] == 'w') ? "walking" : "driving"));

      state->buttons[button] = cmap_action;
    } else {
      if (axis_is_positive)
        state->axis_positive[axis] = cmap_action;
      else
        state->axis_negative[axis] = cmap_action;
    }
  }
  cfg_split_cleanup(split);

  // TODO: remove key from cfg, so we can verify later that there
  // are no invalid keys
}
#undef DOCU
#undef CONVERT_BUTTON

#define MAPPING(IS_REQUIRED, ACTION)                                           \
  cmap_map_action(cmap, cfg, str(ACTION), G2HR_CMAP_##ACTION, IS_REQUIRED)

cmap_t *cmap_init() {
  cmap_t *cmap = calloc(1, sizeof(cmap_t));

  cfg_t *cfg = cfg_load("data/controller-mappings/default.ini", 0);

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
  // controls as for walking.
  MAPPING(0, DRIVING_FORWARD);
  MAPPING(0, DRIVING_BACKWARD);
  MAPPING(0, DRIVING_LEFT);
  MAPPING(0, DRIVING_RIGHT);
  MAPPING(0, DRIVING_ATTACK);
  MAPPING(0, DRIVING_EXIT_CAR);
  MAPPING(0, DRIVING_HANDBRAKE);
  MAPPING(0, DRIVING_WEAPON_PREV);
  MAPPING(0, DRIVING_WEAPON_NEXT);

  cfg_cleanup(cfg);
  return cmap;
}

#undef MAPPING
#undef str

void cmap_cleanup(cmap_t *cmap) { free(cmap); }
