#include "injected_net.h"
#include "../../common/common.h"
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512

// global variable, because we use it in multiple threads
SOCKET global_sock2native = INVALID_SOCKET;

// returns 0 on failure, 1 on success
char net_init() {
  WSADATA wsaData;
  struct addrinfo *result = NULL;
  struct addrinfo *ptr = NULL;
  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return 0;
  }

  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  char port_str[10];
  sprintf(port_str, "%i", G2HR_NATIVE_SERVER_PORT);

  iResult = getaddrinfo("127.0.0.1", port_str, &hints, &result);
  if (iResult) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    return 0;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    global_sock2native =
        socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (global_sock2native == INVALID_SOCKET) {
      printf("socket failed with error: %d\n", WSAGetLastError());
      return 0;
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
    return 0;
  }

  return 1;
}

void net_send(char *buffer, int length) {
  send(global_sock2native, buffer, length, 0);
}

void net_recv_message(char *buffer, int length) {
  recv(global_sock2native, buffer, length, 0);
}

// returns 0 on failure, 1 on success
char net_recv_blocking(void *callback, void *userdata) {
  char msg_id;

  // this call is blocking. it returns <1 on failure
  if (recv(global_sock2native, &msg_id, 1, 0) < 1)
    return 0;

  // execute the callback
  void (*func)(char msg_id, void *userdata) = callback;
  func(msg_id, userdata);

  return 1;
}

void net_cleanup() {
  if (global_sock2native != INVALID_SOCKET)
    closesocket(global_sock2native);

  WSACleanup();
}
