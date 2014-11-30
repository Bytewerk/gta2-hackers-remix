#include "game.h"
#include <stdio.h>
#include <stdlib.h>

// SDL_init and SDL_net_init must be called before!
// try to open as many GTA2 connections, as possible
// also try to map controllers to those GTA2 instances
// TODO: get nickname from proxy_dll (Player 1, Player 2, etc.)
//		and try to map the controller with the same number,
//		so that the glowing number on the Xbox360 controllers
//		actually match the player name
// This returns NULL if no GTA2 instance could be mapped to a gamepad.
// (and does not free ram afterwards - FIXME)
game_t *game_init() {
  int controller_count = SDL_NumJoysticks();
  int controller_index = 0;

  game_t *game = malloc(sizeof(game_t));
  game->player_count = 0;

  printf("%i joystick%s found\n", controller_count,
         (controller_count == 1 ? "" : "s"));

  // Try out ports 19990-19995
  IPaddress localhost;
  TCPsocket sock;
  for (int port = GAMEPADGLUE_START_PORT; port < GAMEPADGLUE_START_PORT + 6;
       port++) {
    printf("Port %i...\n", port);
    SDLNet_ResolveHost(&localhost, "localhost", port);
    sock = SDLNet_TCP_Open(&localhost);

    if (sock != NULL) {
      // TODO: perform cool handshake and display the players
      // name instead of 'connected!'

      printf(" => connected!\n");
      player_t *player = &(game->players[game->player_count]);
      player->mapping = 0;
      player->sock = sock;
      player->pad = NULL;

      for (; controller_index < controller_count && player->pad == NULL;
           controller_index++) {
        if (SDL_IsGameController(controller_index)) {
          player->pad = SDL_GameControllerOpen(controller_index);

          // We need this ID to find out which controller has pressed the guide
          // button
          player->joystick_id = SDL_JoystickInstanceID(
              SDL_GameControllerGetJoystick(player->pad));

          if (player->pad == NULL)
            printf("WARNING: Couldn't connect to controller #%i: %s!\n",
                   controller_index,
                   SDL_JoystickNameForIndex(controller_index));
          else
            printf(" => Assigned controller #%i: %s.\n", controller_index,
                   SDL_GameControllerName(player->pad));
        } else
          printf("NOTE: Controller #%i: %s isn't an XInput compatible gamepad, "
                 "skipping.\n",
                 controller_index, SDL_JoystickNameForIndex(controller_index));
      }

      if (player->pad == NULL)
        printf("ERROR: Couldn't find a gamepad.\n");
      else
        game->player_count++;
    } else
      printf(" => nothing.\n");
  }

  printf("Init complete, enjoy!\n");
  return game->player_count == 0 ? NULL : game;
}
