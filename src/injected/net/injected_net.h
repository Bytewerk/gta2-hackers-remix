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

int net_init(); // >0 on failure
void net_cleanup();

// use SEND2NATIVE instead!
void net_send(char *buffer, int length);
