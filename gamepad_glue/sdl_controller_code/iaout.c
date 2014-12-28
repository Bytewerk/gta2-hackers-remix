#include "game.h"
#include "injected_api.h"
#include "injected_parser.h"
#include <string.h>

void iaout_receive(game_t *game) {
  if (!SDLNet_CheckSockets(game->socket_set, 0))
    return;

  for (int i = 0; i < game->player_count; i++) {
    player_t *player = &(game->players[i]);

    // loop until there's no more data to parse
    while (iaout_parser(player))
      ;
  }
}

// 'break' out of the case if the data is garbage!
#define FRAMEDATACASE(NAME, CODE)                                              \
  case NAME: {                                                                 \
    SDLNet_TCP_Recv(player->sock, &buffer, sizeof(buffer));                    \
    NAME##_t *data = &buffer;                                                  \
    CODE return 1 + sizeof(NAME##_t);                                          \
  }

// returns the count of bytes parsed, 0 on error
int iaout_parser(player_t *player) {
  char header;
  char buffer[200];

  if (!SDLNet_SocketReady(player->sock))
    return 0;
  SDLNet_TCP_Recv(player->sock, &header, 1);

  switch (header) {
    // http://git.io/0x665770
    FRAMEDATACASE(IA_OUT_RUMBLE, {
      if (data->rumble > 20 || data->rumble < -20)
        break;
      SDL_HapticRumblePlay(player->haptic, 1.0, 150 + data->rumble * 50);
    })

    FRAMEDATACASE(IA_OUT_DEBUG_TEXT, {
      if (data->print_hex != 0 && data->print_hex != 1)
        break;

      char debug_text[100];
      strncpy(&debug_text, data->text, 99);
      debug_text[99] = '\0';
      printf("[%i]: %s\n", player->id + 1, debug_text);

      // print hex codes
      if (data->print_hex)
        for (int i = 0; i < 15; i++)
          printf("\ttext[%i]: %c (%x)\n", i, data->text[i], data->text[i]);
    });
  }

  printf("Got garbage from [%i]!\n", player->id + 1);
  return 0;
}
