#include "../injected_api.h"
#include "controller_mapping.h"
#include "game.h"
#include "ia_client.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Disable STDOUT buffering, so we can display the output in the AutoIt3 GUI
  setbuf(stdout, NULL);

  // Add any parameter to this exe file and it will only print the necessary
  // stuff
  int verbose = (argc == 1);

  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());

  if (SDLNet_Init() == -1)
    return printf("ERROR from SDLNet: %s\n", SDLNet_GetError());

  if (verbose) {
    printf("-------------------------------------\n");
    printf("      G A M E P A D G L U E \n");
    printf("-------------------------------------\n");
    printf("Always start GTA2.exe before starting this program.\n");
    printf(
        "This only works, when the proxydll was copied in the GTA2 folder.\n");
    printf("Run this program with any parameter to make it less verbose.\n");
    printf("More info: https://github.com/Bytewerk/gta2-hackers-remix\n\n");
  }

  SDL_GameControllerAddMappingsFromFile("../data/gamecontrollerdb.txt");

  game_t *game = game_init();
  if (game == NULL)
    return printf("ERROR: Couldn't assign any instance to a gamepad!\n");

  printf("[Guide]: Switch layout\n");

  // shake all controllers, so players know we're ready :)
  for (int i = 0; i < game->player_count; i++)
    SDL_HapticRumblePlay(game->players[i].haptic, 1.0, 1000);

  while (1) {
    ia_client_receive(game);

    // Handle game controller activity
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 50);

    // All players: calculate movement bytes from the gamepad
    // input and send it to the associated GTA2 instance
    for (int i = 0; i < game->player_count; i++) {
      player_t *player = &(game->players[i]);
      short ctrl_code = controller_mapping(&e, player);

      // add a frame around the message
      int frame_size = 1 + sizeof(IA_MOVEMENT_t);
      char *frame = malloc(frame_size);
      frame[0] = IA_MOVEMENT;

      IA_MOVEMENT_t *data = (IA_MOVEMENT_t *)(frame + 1);
      data->movement = ctrl_code;

      SDLNet_TCP_Send(player->sock, frame, frame_size);

      free(frame);
    }
  }
  return 0;
}
