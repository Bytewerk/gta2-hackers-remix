#include "controller_mapping.h"
#include "game.h"
#include "injected_api.h"
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

  // create a socket set over all sockets, so we can check them for activity
  SDLNet_SocketSet set = SDLNet_AllocSocketSet(game->player_count);
  for (int i = 0; i < game->player_count; i++)
    SDLNet_AddSocket(set, game->players[i].sock);

  // shake all gamepads, so they know we're ready :)
  for (int i = 0; i < game->player_count; i++)
    SDL_HapticRumblePlay(game->players[i].haptic, 1.0, 1000);

  while (1) {
    // Handle incoming TCP data, if any
    if (SDLNet_CheckSockets(set, 0))
      for (int i = 0; i < game->player_count; i++) {
        player_t *player = &(game->players[i]);
        if (!SDLNet_SocketReady(player->sock))
          continue;

        char buffer[200];
        SDLNet_TCP_Recv(player->sock, buffer, sizeof(buffer));
        switch (buffer[0]) {
        // https://github.com/Bytewerk/gta2-hackers-remix/wiki/0x665770-Rumble-Byte
        case IA_OUT_RUMBLE: {
          char rumble = buffer[1];
          int duration = 150 + rumble * 50;
          SDL_HapticRumblePlay(player->haptic, 1.0, duration);
          // printf("rumble: %i; duration: %i\n", buffer[1], duration);
          break;
        }
        case IA_OUT_DEBUG_TEXT: {
          char print_hex = buffer[1];
          char *text = buffer + 2;
          printf("[%i]: %s\n", i + 1, text);
          if (print_hex)
            for (int j = 2; j < 17; j++)
              printf("\tbuffer[%i]: %c (%x)\n", j, buffer[j], buffer[j]);
          break;
        }
        default: {
          printf("ERROR: got junk from player %i's GTA2 instance:\n", i + 1);
          // for(int j=0;j<10;j++)
          //    printf("\tbuffer[%i]: %c (%x)\n", j, buffer[j], buffer[j]);
        }
        }
      }

    // Handle game controller activity
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 50);

    // All players: calculate movement bytes from the gamepad
    // input and send it to the associated GTA2 instance
    for (int i = 0; i < game->player_count; i++) {
      player_t *player = &(game->players[i]);
      short ctrl_code = controller_mapping(&e, player);

      // add a frame around the message
      int frame_size = 1 + sizeof(IA_IN_MOVEMENT_t);
      char *frame = malloc(frame_size);
      frame[0] = IA_IN_MOVEMENT;

      IA_IN_MOVEMENT_t *data = (IA_IN_MOVEMENT_t *)(frame + 1);
      data->movement = ctrl_code;

      SDLNet_TCP_Send(player->sock, frame, frame_size);

      free(frame);
    }
  }

  // TODO: Cleanup code!
  // SDL_GameControllerClose(game->pad);
  // SDL_GameControllerClose(game2->pad);
  SDL_Quit();

  return 0;
}
