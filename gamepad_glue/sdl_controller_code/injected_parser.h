#pragma once

/*
    This macro should be used in the parser's switch
    statement.

    'break' away from the switch...case if the data
    is garbage and it will say so on the terminal.
*/
#define FRAMEDATACASE(NAME, CODE)                                              \
  case NAME: {                                                                 \
    SDLNet_TCP_Recv(player->sock, &buffer, sizeof(buffer));                    \
    NAME##_t *data = &buffer;                                                  \
    CODE return 1 + sizeof(NAME##_t);                                          \
  }
