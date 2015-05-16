#pragma once
#include "../../common/api_native2injected.h"

/*
        Sends "api_native2injected.h" messages to the native component.
        Use the CODE parameter to fill the data array.

        Example useage:
                SEND2NATIVE(IA_RUMBLE, data->rumble=123 );
                SEND2NATIVE(IA_SCOREBOARD,
                {
                        for(int i=0;i<5;i++)
                                data->score[i] = data->money[i] = i*10;
                });

*/
#define SEND2NATIVE(MSG_ID, CODE)                                              \
  {                                                                            \
    char buffer[sizeof(MSG_ID##_t) + 1];                                       \
    buffer[0] = MSG_ID;                                                        \
    MSG_ID##_t *data = (MSG_ID##_t *)(buffer + 1);                             \
    CODE;                                                                      \
    net_send(buffer, sizeof(buffer));                                          \
  }

// see mem_recv_callback() in mem/mem.c for example usage
#define FROMNATIVECASE(MSG_ID, CODE)                                           \
  case MSG_ID: {                                                               \
    char buffer[sizeof(MSG_ID##_t)];                                           \
    net_recv_message(buffer, sizeof(MSG_ID##_t));                              \
    MSG_ID##_t *data = (MSG_ID##_t *)buffer;                                   \
    CODE;                                                                      \
    return;                                                                    \
  }

// returns 0 on failure
char net_init();

// returns 0 on failure
char net_recv_blocking(void *callback, void *userdata);

void net_cleanup();

// use SEND2NATIVE and FROMNATIVECASE instead!
void net_send(char *buffer, int length);
void net_recv_message(char *buffer, int length);
