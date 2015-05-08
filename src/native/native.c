#include "meh/meh.h"
#include "net/native_net.h"
#include "pads/pads.h"
#include <stdio.h>
#include <string.h>

// TODO: split the Linux part up: run the wine prefix setup
// as blocking process, then run the menu in the background
void menu_start(int server_port, char menu_compiled_for_linux) {
  char *buffer = malloc(100);

  if (menu_compiled_for_linux) {
    // run the menu with gdb, print a stack trace when it fails
    snprintf(buffer, 100,
             "gdb -batch -ex run -ex bt --args src/menu/out/menu.bin %i &",
             server_port);
  } else if (!strcmp(SDL_GetPlatform(), "Windows"))
    snprintf(buffer, 100, "start bin/menu.exe %i", server_port);
  else
    snprintf(buffer, 100, "bin/wine_wrapper.sh %i &", server_port);

  system(buffer);
  free(buffer);
}

int main(int argc, char **argv) {
  native_t *native = calloc(1, sizeof(native_t));

  // initialize everything
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());
  net_t *net = net_init();
  pads_t *pads = pads_init(0);
  menu_start(net->port,
             (argc == 2 && !strcmp(argv[1], "--debug-menu-with-gdb-on-linux")));

  // TODO: init controllers etc.

  // wait up to 20 seconds for the menu connection. If it fails,
  // quit (the menu will show an error message)
  net_block_until_connected(net, 20000);
  if (!net->sock_menu) {
    printf("META: NOT CONNECTED TO MENU!\n");
    native->quit = 1;
  }

  // DEBUG!

  while (!native->quit) {
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 50);
    net_frame(net, native);
    pads_frame(pads, &e, 0);
    if (!native->is_ingame)
      meh_frame(net, pads, &e);

    // net_send_menu_action(net, TK_ACTION_UP, 1);
  }

  // clean up
  net_cleanup(net);
  pads_cleanup(pads);
  free(native);
  SDL_Quit();
}
