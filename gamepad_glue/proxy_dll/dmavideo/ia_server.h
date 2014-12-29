#pragma once
#include <winsock2.h>

void ia_server_log(SOCKET ClientSocket, char print_hex, const char *format,
                   ...);

int ia_server_parser(SOCKET ClientSocket, char header);

void ia_server_rumble_byte(SOCKET ClientSocket);
