#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>

/*
    TODO:
        - Make use of SDL_IsGameController, before attempting to open
        - Depending on argc/argv, start an own TCP server (for interaction with
   the menu)
        - Maybe move the server to an own thread
        - Allow switching between different controller mappings
        - Support for multiple controllers
            (should work in a single thread: open all controllers and
             tcp connections to the proxy dlls, and in the main loop,
             iterate through all controllers!)
*/

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
    return printf("SDL could not initialize!\nSDL Error: %s\n", SDL_GetError());

  if (SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    exit(2);
  }

  game_t *game = malloc(sizeof(game_t));
  game_init(game, 1);

  printf("Stuff initialized!\n");

  while (1) {
    SDL_Event e;
    if (!SDL_WaitEvent(&e))
      continue;

    short ctrl_code = controller_mapping(game);
    SDLNet_TCP_Send(game->sock, &ctrl_code, sizeof(short));
  }
  free(game);

  SDL_GameControllerClose(game->pad);
  SDL_Quit();

  return 0;
}
