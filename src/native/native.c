#include "net/net.h"
#include <stdio.h>

// TODO: split the Linux part up: run the wine prefix setup
// as blocking process, then run the menu in the background
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

  // TODO: init controllers etc.

  // wait up to 20 seconds for the menu connection. If it fails,
  // quit (the menu will show an error message)
  net_block_until_connected(net, 20000);
  if (!net->sock_menu)
    printf("META: NOT CONNECTED TO MENU!\n");

  // TODO:  else while(1) .... main loop!

  // clean up
  net_cleanup(net);
}
