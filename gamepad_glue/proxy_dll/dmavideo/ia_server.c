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
    NAME##_t *data = (NAME##_t *)(buffer + 1);                                 \
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
// Max. length of the string is in injected_api.h.
void ia_server_log(SOCKET ClientSocket, const char *format, ...) {
  va_list args;

  FRAMEDATASEND(IA_DEBUG_TEXT, {
    va_start(args, format);
    vsprintf_s(data->text, sizeof(data->text), format, args);
    va_end(args);
  });
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

void ia_server_score_change(SOCKET ClientSocket) {
  static int score = 0;
  if (*GTA2_ADDRSP_SCORE != score) {
    int delta = *GTA2_ADDRSP_SCORE - score;
    if (delta > 0)
      FRAMEDATASEND(IA_SCORE_DELTA, data->delta = delta);
    score = *GTA2_ADDRSP_SCORE;
  }
}

void ia_server_send_info(SOCKET ClientSocket) {
  ia_server_player_info(ClientSocket);
  ia_server_rumble_byte(ClientSocket);
  // ia_server_score_change(ClientSocket);
}

// returns the count of bytes parsed, 0 on error
int ia_server_parser(SOCKET ClientSocket, char header) {
  char buffer[200];

  switch (header) {
    FRAMEDATACASE(IA_MOVEMENT,
                  { memcpy(GTA2_ADDR_MOVEMENT, &(data->movement), 2); });
  }

  // send the 5 first bytes back
  ia_server_log(ClientSocket, "Got garbage: %x %x %x %x %x", header & 0xff,
                buffer[0] & 0xff, buffer[1] & 0xff, buffer[2] & 0xff,
                buffer[3] & 0xff);
  return 0;
}
