#include "controller_mapping.h"
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
    return printf("ERROR from SDL: %s\n", SDL_GetError());

  if (SDLNet_Init() == -1)
    return printf("ERROR from SDLNet: %s\n", SDLNet_GetError());
  printf("-------------------------------------\n");
  printf("      G A M E P A D G L U E \n");
  printf("-------------------------------------\n");
  printf("Always start GTA2.exe before starting this program.\n");
  printf("This only works, when the proxydll was copied in the GTA2 folder.\n");
  printf("More info: https://github.com/Bytewerk/gta2-hackers-remix\n\n");

  game_t *game = game_init();
  if (game == NULL)
    return printf("ERROR: Couldn't assign any instance to a gamepad!\n");

  printf("Press the guide button to cycle through the controller mappings!\n");

  while (1) {
    SDL_Event e;
    if (!SDL_WaitEvent(&e))
      continue;

    // All players: calculate movement bytes from the gamepad
    // input and send it to the associated GTA2 instance
    for (int i = 0; i < game->player_count; i++) {
      player_t *player = &(game->players[i]);
      short ctrl_code = controller_mapping(&e, player);
      SDLNet_TCP_Send(player->sock, &ctrl_code, sizeof(short));
    }
  }

  // TODO: Cleanup code!
  // SDL_GameControllerClose(game->pad);
  // SDL_GameControllerClose(game2->pad);
  SDL_Quit();

  return 0;
}
