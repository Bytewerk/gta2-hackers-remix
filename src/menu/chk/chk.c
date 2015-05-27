#include <SDL2/SDL_net.h>

// if the connection fails, this thread quits silently
int update_check_thread(void *userdata) {
  // char* settings_path = (char*) userdata;

  // todo: load settings and read the last update check date

  // resulve the IP
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, "localhost", 80) < 0)
    return 0;

  TCPsocket sock = SDLNet_TCP_Open(&ip);
  if (!sock)
    return 0;

  char *get = "GET /version";
  SDLNet_TCP_Send(sock, get, strlen(get) + 1);

  char buffer[100];
  SDLNet_TCP_Recv(sock, buffer, 99);

  printf("ret: %s\n", buffer);

  return 0;
}

void update_check_in_background(char *settings_path) {
  SDL_DetachThread(SDL_CreateThread(&update_check_thread, "updatecheck",
                                    (void *)settings_path));
}
