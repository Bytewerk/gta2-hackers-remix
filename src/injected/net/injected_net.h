#pragma once
#include "../../common/headers/api_native2injected.h"

/*
        The message macros are identically to the ones in
        net_message_macros_SDL.h, except that they don't have a SOCKET
        parameter here and (indirectly) use the microsoft API instead of
        SDL_net.
*/

/*
        Sends "api_native2injected.h" messages to the native component.
        Use the CODE parameter to fill the data array.

        Example useage:
                MESSAGESEND(IA_RUMBLE, data->rumble=123 );
                MESSAGESEND(IA_SCOREBOARD,
                {
                        for(int i=0;i<5;i++)
                                data->score[i] = data->money[i] = i*10;
                });

*/
#define MESSAGESEND(MSG_ID, CODE)                                              \
  {                                                                            \
    char buffer[sizeof(MSG_ID##_t) + 1];                                       \
    buffer[0] = MSG_ID;                                                        \
    MSG_ID##_t *data = (MSG_ID##_t *)(buffer + 1);                             \
    CODE;                                                                      \
    net_send(buffer, sizeof(buffer));                                          \
  }

#define MESSAGESENDSHORT(MSG_ID)                                               \
  {                                                                            \
    char header = MSG_ID;                                                      \
    net_send(&header, 1);                                                      \
  }

// see mem_recv_callback() in mem/mem.c for example usage
#define MESSAGECASE(MSG_ID, CODE)                                              \
  case MSG_ID: {                                                               \
    char buffer[sizeof(MSG_ID##_t)];                                           \
    net_recv_message(buffer, sizeof(MSG_ID##_t));                              \
    MSG_ID##_t *data = (MSG_ID##_t *)buffer;                                   \
    CODE;                                                                      \
    return;                                                                    \
  }

#define MESSAGECASESHORT(MSG_ID, CODE)                                         \
  case MSG_ID: {                                                               \
    CODE;                                                                      \
    return;                                                                    \
  }

// returns 0 on failure
char net_init();

// returns 0 on failure
char net_recv_blocking(void *callback, void *userdata);

void net_cleanup();

// use MESSAGESEND and MESSAGECASE instead!
void net_send(char *buffer, int length);
void net_recv_message(char *buffer, int length);
