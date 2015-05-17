#include "../common/common.h"
#include "cmap/cmap.h"
#include "ingame/ingame.h"
#include "inmenu/inmenu.h"
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

  printf("[native] starting menu component...\n");
  system(buffer);
  free(buffer);
}

int main(int argc, char **argv) {
  // initialize everything
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());
  cmap_t *cmap = cmap_init();
  pads_t *pads = pads_init(0);
  net_t *net = net_init();
  inmenu_t *inmenu = inmenu_init(net, pads);
  ingame_t *ingame = ingame_init(net, cmap, pads, inmenu);

  // wait up to 20 seconds for the menu connection. If it fails,
  // quit (the menu will show an error message)
  menu_start(G2HR_NATIVE_SERVER_PORT,
             (argc == 2 && !strcmp(argv[1], "--debug-menu-with-gdb-on-linux")));
  net_block_until_connected(net, 20000);
  if (!net->sock_menu) {
    printf("[native] not connected to menu!\n");
    inmenu->has_quit = 1;
  }

  while (!inmenu->has_quit) {
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 50);

    pads_frame(pads, &e, 0);

    if (!net_frame(net, (void *)inmenu_recv_callback, (void *)inmenu,
                   (void *)ingame_recv_callback, (void *)ingame))
      break;

    if (net->injected_count)
      ingame_frame(ingame, &e);
    else
      inmenu_frame(inmenu, &e);
  }

  // clean up
  ingame_cleanup(ingame);
  inmenu_cleanup(inmenu);
  net_cleanup(net);
  pads_cleanup(pads);
  cmap_cleanup(cmap);
  SDL_Quit();
}
