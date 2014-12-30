#include "../../injected_api.h"
#include "gta2_memory.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

// fill 'data' in the CODE section
#define FRAMEDATASEND(NAME, CODE)                                              \
  {                                                                            \
    char buffer[sizeof(NAME##_t) + 1];                                         \
    buffer[0] = NAME;                                                          \
    NAME##_t *data = ((NAME##_t *)&buffer) + 1;                                \
    CODE;                                                                      \
    send(ClientSocket, buffer, sizeof(buffer), 0);                             \
  }

// do something with 'data' in the CODE section
// 'break' out of the case if the data is garbage!
#define FRAMEDATACASE(NAME, CODE)                                              \
  case NAME: {                                                                 \
    recv(ClientSocket, buffer, sizeof(NAME##_t), 0);                           \
    NAME##_t *data = (NAME##_t *)&buffer;                                      \
    CODE return 1 + sizeof(NAME##_t);                                          \
  }

// This is a debug message that can send a string down the
// pipe to the connected sdl_contoller_code (which should
// in turn print it on stdout with the player instance number).
// Format and the remaining arguments can be used like in printf.
// Make sure that the message length is max. ~90 characters!
void ia_server_log(SOCKET ClientSocket, char print_hex, const char *format,
                   ...) {
  va_list args;

  char buffer[200];
  buffer[0] = IA_DEBUG_TEXT;
  buffer[1] = print_hex;

  va_start(args, format);
  vsprintf(buffer + 2, format, args);
  va_end(args);

  send(ClientSocket, buffer, sizeof(buffer), 0);
}

void ia_server_player_info(SOCKET ClientSocket) {
  // This one will be useful for rumble (running over
  // enemies in vehicles?) and for different controller
  // layouts for driving in vehicles
  static char in_vehicle = 0x00;
  if (in_vehicle != *GTA2_ADDR_PLAYER_IN_VEHICLE)
    FRAMEDATASEND(IA_VEHICLE_INFO,
                  data->in_vehicle = in_vehicle = *GTA2_ADDR_PLAYER_IN_VEHICLE);

  // TODO: send which weapon the player is using etc.
}

// Edge detection for the rumble byte
// More info: http://git.io/g2hr-rumble-byte
void ia_server_rumble_byte(SOCKET ClientSocket) {
  if (*GTA2_ADDR_RUMBLE != 0x00) {
    char rumble = *GTA2_ADDR_RUMBLE;
    if (rumble > 0)
      FRAMEDATASEND(IA_RUMBLE, data->rumble = rumble);

    // Sometimes the game doesn't clean up this value!
    // so make sure that we reset it
    *GTA2_ADDR_RUMBLE = 0x00;
  }
}

void ia_server_send_info(SOCKET ClientSocket) {
  ia_server_player_info(ClientSocket);
  ia_server_rumble_byte(ClientSocket);
}

// returns the count of bytes parsed, 0 on error
int ia_server_parser(SOCKET ClientSocket, char header) {
  char buffer[200];

  switch (header) {
    FRAMEDATACASE(IA_MOVEMENT,
                  { memcpy(GTA2_ADDR_MOVEMENT, &(data->movement), 2); });
  }

  static int garbage_counter = 0;
  garbage_counter++;

  if ((garbage_counter + 99) % 100 == 0)
    ia_server_log(ClientSocket, 0, "garbage_counter: %i!", garbage_counter);
  return 0;
}
