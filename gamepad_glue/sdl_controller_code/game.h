#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define GAMEPADGLUE_MAPPINGS_COUNT 3
#define GAMEPADGLUE_START_PORT 19990

typedef struct {
  SDL_GameController *pad;
  SDL_Joystick *joystick;
  SDL_JoystickID joystick_id; // needed to access the GUIDE button
  SDL_Haptic *haptic;         // rumble / force feedback
  int mapping;
  TCPsocket sock;
} player_t;

typedef struct {
  int player_count;

  // this doesn't take up much space
  // and is less complex than a chained
  // list
  player_t players[6];

} game_t;

game_t *game_init();
