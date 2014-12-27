/*
	This file is heavily copypasted from this tcp server example:
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms737593%28v=vs.85%29.aspx

	Basically we listen on ports 19990-19995 (depending on which one works, think
	of up to 6 GTA2 instances!), until a connection is established. As soon, as we
	get data, we stuff 2 bytes at a time to the address, where the current movement
	is stored (overwriting the previous two).
	
	When a connection goes down, this program tries the next port (this is useful
	when we want to change the sdl_client code: no gta2 restart required!)
	
	All of this works surprisingly well :)
*/

#undef UNICODE
#define WIN32_LEAN_AND_MEAN // what the hell?

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "gta2_memory.h"
#include "injected_api.h"


// Speed up the build process
// http://stackoverflow.com/a/11040290
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 200
#define TCP_SERVER_START_PORT 19990


// Returns NULL (listen or connection fails!)
// or the client socket, if the connection was successful.
SOCKET injected_thread_listen(int port)
{
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;

	// TODO: find better names for these two!
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
	char port_string[10];
	_itoa(port,port_string,10);

    iResult = getaddrinfo(NULL, port_string, &hints, &result);

    // Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) return NULL; // TODO: display an error here

    // Setup the TCP listening socket
	// If it fails, the outer loop will try the next port.
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
	
	if (iResult == SOCKET_ERROR)
	{
        closesocket(ListenSocket);
		return NULL;
    }

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
	{
        closesocket(ListenSocket);
        WSACleanup();
        return NULL;
    }

    // Accept a client socket
	// TODO: perform some sort of simple handshake here?
	SOCKET ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
		return NULL;

    // No longer need server socket
    closesocket(ListenSocket);

	return ClientSocket;
}


/*
	Print debug messages like this:

	char buffer[100];
	sprintf(buffer, "New Rumble: %i", rumble);
	MessageBox(NULL, buffer, "G2HR", NULL);
*/

// returns 1 if receiving still works
// returns 0 if the connection is dead
int injected_thread_receive(SOCKET ClientSocket)
{
    char recvbuf[DEFAULT_BUFLEN];

	// Wait until the client has a new message. This happens at least every 50ms,
	// see SDL_WaitEventTimeout() in main.c of sdl_controller_code.
	// Also return on disconnect.
	if (recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0) <= 0)
		return 0;

	// The message type is the first byte
	switch (recvbuf[0])
	{
		case IA_IN_MOVEMENT:
		{
			memcpy(GTA2_ADDR_MOVEMENT, recvbuf + 1, 2);
			break;
		}
	}

	// Edge detection for the rumble byte
	// https://github.com/Bytewerk/gta2-hackers-remix/wiki/0x665770-Rumble-Byte
	static char rumble = 0x00;
	if(rumble != *GTA2_ADDR_RUMBLE)
	{
		rumble = *GTA2_ADDR_RUMBLE;

		if (rumble > 0)
		{
			char buffer[2];
			buffer[0] = IA_OUT_RUMBLE;
			buffer[1] = rumble;
			send(ClientSocket, buffer, sizeof(buffer), 0);
		}

		// Sometimes the game doesn't clean up this value!
		// so make sure that we reset it
		// TODO: make sure this gets initialized to 0x00
		*GTA2_ADDR_RUMBLE = 0x00;
	}

	return 1; // everything's fine
}


void __cdecl injected_thread(void* param)
{
	// Initialize WinSock
	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2,2), &wsa_data);

	while(1)
	{
		// try ports from 19990 to 19995
		for(int i=0;i<6;i++)
		{
			SOCKET ClientSocket = injected_thread_listen(TCP_SERVER_START_PORT + i);
			if(ClientSocket == NULL) continue;

			while(injected_thread_receive(ClientSocket));
		}

		Sleep(100); // Wait 100ms
	}
	WSACleanup();
}

