#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define GAMEPADGLUE_MAPPINGS_COUNT 3
#define GAMEPADGLUE_START_PORT 19990

typedef struct {
  char id;
  char mapping;
  SDL_GameController *pad;
  SDL_Joystick *joystick;
  SDL_JoystickID joystick_id; // needed to access the GUIDE button
  SDL_Haptic *haptic;         // rumble / force feedback
  TCPsocket sock;
} player_t;

typedef struct {
  char player_count;
  player_t players[6];
  SDLNet_SocketSet socket_set;
} game_t;

game_t *game_init();
