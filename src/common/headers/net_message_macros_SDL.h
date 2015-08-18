#pragma once
#include <SDL2/SDL_net.h>

#define MESSAGESEND(SOCKET, MSG_ID, CODE)                                      \
  {                                                                            \
    char buffer[sizeof(MSG_ID##_t) + 1] = {0};                                 \
    buffer[0] = MSG_ID;                                                        \
    MSG_ID##_t *data = (MSG_ID##_t *)(buffer + 1);                             \
    CODE;                                                                      \
    SDLNet_TCP_Send(SOCKET, buffer, sizeof(buffer));                           \
  }

#define MESSAGESENDSHORT(SOCKET, MSG_ID)                                       \
  {                                                                            \
    char header = MSG_ID;                                                      \
    SDLNet_TCP_Send(SOCKET, &header, 1);                                       \
  }

#define MESSAGECASE(SOCKET, MSG_ID, CODE)                                      \
  case MSG_ID: {                                                               \
    char buffer[sizeof(MSG_ID##_t)];                                           \
    SDLNet_TCP_Recv(SOCKET, buffer, sizeof(MSG_ID##_t));                       \
    MSG_ID##_t *data = (MSG_ID##_t *)buffer;                                   \
    CODE;                                                                      \
    return;                                                                    \
  }

#define MESSAGECASESHORT(MSG_ID, CODE)                                         \
  case MSG_ID: {                                                               \
    CODE;                                                                      \
    return;                                                                    \
  }
