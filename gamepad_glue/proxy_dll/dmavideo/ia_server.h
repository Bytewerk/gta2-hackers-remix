#pragma once
#include <winsock2.h>

void ia_server_log(SOCKET ClientSocket, const char *format, ...);

int ia_server_parser(SOCKET ClientSocket, char header);

void ia_server_send_info(SOCKET ClientSocket);