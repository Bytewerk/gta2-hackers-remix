#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define GAMEPADGLUE_MAPPINGS_COUNT 3
#define GAMEPADGLUE_START_PORT 27015

typedef struct {
  SDL_GameController *pad;
  int mapping;
  TCPsocket sock;
} game_t;
