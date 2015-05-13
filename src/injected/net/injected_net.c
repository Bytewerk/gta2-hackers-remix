#include "injected_net.h"
#include "../../common/common.h"
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512

// global variable, because we use it in multiple threads
SOCKET global_sock2native = INVALID_SOCKET;

int net_init() {
  WSADATA wsaData;

  struct addrinfo *result = NULL, *ptr = NULL, hints;

  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return 1;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  char port_str[10];
  sprintf(port_str, "%i", G2HR_NATIVE_SERVER_PORT);

  iResult = getaddrinfo("127.0.0.1", port_str, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return 1;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    global_sock2native =
        socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (global_sock2native == INVALID_SOCKET) {
      printf("socket failed with error: %d\n", WSAGetLastError());
      WSACleanup();
      return 1;
    }

    // Connect to server.
    iResult = connect(global_sock2native, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(global_sock2native);
      global_sock2native = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (global_sock2native == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    return 1;
  }
  /*
// Send an initial buffer
iResult = send(global_sock2native, sendbuf, (int)strlen(sendbuf), 0 );
if (iResult == SOCKET_ERROR) {
  printf("send failed with error: %d\n", WSAGetLastError());
  closesocket(global_sock2native);
  WSACleanup();
  return 1;
}

printf("Bytes Sent: %i\n", iResult);

// shutdown the connection since no more data will be sent
iResult = shutdown(global_sock2native, SD_SEND);
if (iResult == SOCKET_ERROR) {
  printf("shutdown failed with error: %d\n", WSAGetLastError());
  closesocket(global_sock2native);
  WSACleanup();
  return 1;
}


char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

// Receive until the peer closes the connection
do {

  iResult = recv(global_sock2native, recvbuf, recvbuflen, 0);
  if ( iResult > 0 )
      printf("Bytes received: %d\n", iResult);
  else if ( iResult == 0 )
      printf("Connection closed\n");
  else
      printf("recv failed with error: %d\n", WSAGetLastError());

} while( iResult > 0 );
  */

  return 0;
}

void net_send(char *buffer, int length) {
  send(global_sock2native, buffer, length, 0);
}

void net_cleanup() {
  if (global_sock2native != INVALID_SOCKET) {
    closesocket(global_sock2native);
  }

  WSACleanup();
}
