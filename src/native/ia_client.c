#include "../injected_api.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// do something with 'data' in the CODE section
// 'break' out of the case if the data is garbage!
#define FRAMEDATACASE(NAME, CODE)                                              \
  case NAME: {                                                                 \
    SDLNet_TCP_Recv(player->sock, &buffer, sizeof(NAME##_t));                  \
    NAME##_t *data = (NAME##_t *)&buffer;                                      \
    CODE return 1 + sizeof(NAME##_t);                                          \
  }

// returns the count of bytes parsed, 0 on error
int ia_client_parser(player_t *player, char header) {
  // pre-fill the buffer to make debugging easier
  char buffer[200];
  for (int i = 0; i < sizeof(buffer); i++)
    buffer[i] = 0x42;

  switch (header) {
    // http://git.io/0x665770
    FRAMEDATACASE(IA_RUMBLE, {
      if (data->rumble > 20 || data->rumble < -20)
        break;
      SDL_HapticRumblePlay(player->haptic, 1.0, 150 + data->rumble * 50);
    });

    FRAMEDATACASE(IA_DEBUG_TEXT, {
      data->text[sizeof(data->text) - 2] = '\0';
      printf("#%i> %s\n", player->id + 1, data->text);
    });

    FRAMEDATACASE(IA_VEHICLE_INFO, {
      if (data->in_vehicle != 0 && data->in_vehicle != 1)
        break;

      player->in_vehicle = data->in_vehicle;
      printf("#%i: %s\n", player->id + 1,
             player->in_vehicle ? "entered a vehicle" : "left a vehicle");
    });

    FRAMEDATACASE(IA_API_VERSION, {
      if (data->version != INJECTED_API_VERSION)
        printf("#%i ERROR: Injected API version mismatch (client: %i, server: "
               "%i)!\n",
               player->id + 1, INJECTED_API_VERSION, data->version);
      else
        printf("#%i: Connection initialized!\n", player->id + 1);
    });

    // TODO: make this more generic
    FRAMEDATACASE(IA_SCORE_DELTA, {
      if (player->in_vehicle && data->delta < 500)
        SDL_HapticRumblePlay(player->haptic, 1.0, 300);
    });
  }

  printf("#%i GARBAGE:    %02x", player->id + 1, header & 0xff);
  for (int i = 0; i < 15; i++)
    printf("%s %02x", (i % 5 == 0) ? " |" : "", buffer[i] & 0xff);
  printf("\n");

  return 0;
}

void ia_client_receive(game_t *game) {
  if (!SDLNet_CheckSockets(game->socket_set, 0))
    return;

  for (int i = 0; i < game->player_count; i++) {
    char header;
    player_t *player = &(game->players[i]);

    // loop until there's no more data to parse
    do {
      if (!SDLNet_SocketReady(player->sock))
        break;

      // receive the next error byte or shutdown
      // if the connection is dead!
      if (SDLNet_TCP_Recv(player->sock, &header, 1) <= 0) {
        printf("#%i: Socket disconnected. Shutting down...\n", player->id + 1);
        game_cleanup(game);
        exit(0);
      }
    } while (ia_client_parser(player, header));
  }
}
