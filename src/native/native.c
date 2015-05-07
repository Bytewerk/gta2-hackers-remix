#include "net/net.h"
#include <stdio.h>

void menu_start(int server_port) {
  char *buffer = malloc(100);

  if (!strcmp(SDL_GetPlatform(), "Windows"))
    snprintf(buffer, 100, "start bin/menu.exe %i", server_port);
  else
    snprintf(buffer, 100, "bin/wine_wrapper.sh %i &", server_port);

  system(buffer);
  free(buffer);
}

int main(int argc, char **argv) {
  net_t *net = net_init();
  menu_start(net->port);

  // TODO: main loop here

  // clean up
  net_cleanup(net);
}
