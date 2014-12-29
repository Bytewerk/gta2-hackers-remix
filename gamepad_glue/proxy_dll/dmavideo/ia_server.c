#include "../../injected_api.h"
#include "gta2_memory.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

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

// Edge detection for the rumble byte
// More info: http://git.io/g2hr-rumble-byte
void ia_server_rumble_byte(SOCKET ClientSocket) {
  static char rumble = 0x00;
  if (rumble != *GTA2_ADDR_RUMBLE) {
    rumble = *GTA2_ADDR_RUMBLE;

    if (rumble > 0) {
      char buffer[2];
      buffer[0] = IA_RUMBLE;
      buffer[1] = rumble;
      send(ClientSocket, buffer, sizeof(buffer), 0);
    }

    // Sometimes the game doesn't clean up this value!
    // so make sure that we reset it
    *GTA2_ADDR_RUMBLE = 0x00;
  }
}

// 'break' out of the case if the data is garbage!
#define FRAMEDATACASE(NAME, CODE)                                              \
  case NAME: {                                                                 \
    recv(ClientSocket, buffer, sizeof(NAME##_t), 0);                           \
    NAME##_t *data = (NAME##_t *)&buffer;                                      \
    CODE return 1 + sizeof(NAME##_t);                                          \
  }

// returns the count of bytes parsed, 0 on error
int ia_server_parser(SOCKET ClientSocket, char header) {
  char buffer[200];
  // receive header byte

  switch (header) {
    FRAMEDATACASE(IA_MOVEMENT,
                  { memcpy(GTA2_ADDR_MOVEMENT, &(data->movement), 2); })
  }

  static int garbage_counter = 0;
  garbage_counter++;

  if ((garbage_counter + 99) % 100 == 0)
    ia_server_log(ClientSocket, 0, "garbage_counter: %i!", garbage_counter);
  return 0;
}
