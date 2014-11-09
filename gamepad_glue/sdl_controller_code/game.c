#include "game.h"

// SDL_init and SDL_net_init must be called before!
void game_init(game_t *game, int player) {
  // Set the default control-mapping. This can be toggled
  // with the big Xbox360-button in the future.
  game->mapping = 0;

  // Try to open all gamepads in a loop until one really
  // works - or die
  int joystick_count = SDL_NumJoysticks();
  int pad_connected = 0;
  for (int i = 0; i < joystick_count && !pad_connected; i++) {
    if (!SDL_IsGameController(i))
      continue;
    game->pad = SDL_GameControllerOpen(i);
    pad_connected = (game->pad != NULL);
  }
  if (!pad_connected) {
    printf("Couldn't find a gamepad for player %i, exiting!\n", player);
    exit(1);
  }

  // Try to connect to a socket in a loop, until one works
  // - or die
  int tcp_connected = 0;
  IPaddress localhost;
  for (int port = GAMEPADGLUE_START_PORT;
       port < GAMEPADGLUE_START_PORT + 3 && !tcp_connected; port++) {
    printf("(Player %i) Trying port %i...\n", player, port);
    SDLNet_ResolveHost(&localhost, "localhost", port);
    game->sock = SDLNet_TCP_Open(&localhost);
    tcp_connected = game->sock != NULL;
  }
  if (!tcp_connected) {
    printf("Failed to connect to proxydll of game %i, exiting!\n", player);
    exit(2);
  }
  printf("Prepared gamepad and TCP connection for player %i\n", player);
}
