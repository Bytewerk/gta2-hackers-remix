#include "../common/headers/common.h"
#include "cmap/cmap.h"
#include "ingame/ingame.h"
#include "inmenu/inmenu.h"
#include "net/native_net.h"
#include "pads/pads.h"
#include <stdio.h>
#include <string.h>

#define CMDBUF_LEN 100
// TODO: split the Linux part up: run the wine prefix setup
// as blocking process, then run the menu in the background
void menu_start(char *arg) {
  char cmdline[CMDBUF_LEN];

  if (!strcmp(arg, "--menu-gdb-mixed"))
    snprintf(cmdline, CMDBUF_LEN, "gdb -batch -ex run -ex bt"
                                  " --args src/menu/out/menu.bin %i &",
             G2HR_NATIVE_SERVER_PORT);

  else if (!strcmp(arg, "--menu-gdb-wine"))
    snprintf(cmdline, CMDBUF_LEN, "bin/wine_wrapper.sh %i --gdb"
                                  " &",
             G2HR_NATIVE_SERVER_PORT);

  else if (!strcmp(SDL_GetPlatform(), "Windows"))
    snprintf(cmdline, CMDBUF_LEN, "start bin/menu.exe %i",
             G2HR_NATIVE_SERVER_PORT);
  else
    snprintf(cmdline, CMDBUF_LEN, "bin/wine_wrapper.sh %i &",
             G2HR_NATIVE_SERVER_PORT);

  printf("[native] starting menu component...\n");
  printf("> %s\n", cmdline);
  system(cmdline);
}
#undef CMDBUF_LEN

int main(int argc, char **argv) {
  // initialize everything
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_TIMER) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());
  cmap_t *cmap = cmap_init();
  pads_t *pads = pads_init(0);
  net_t *net = net_init();
  inmenu_t *inmenu = inmenu_init(net, pads);
  ingame_t *ingame = ingame_init(net, cmap, pads, inmenu);

  // wait up to 20 seconds for the menu connection. If it fails,
  // quit (the menu will show an error message)
  menu_start((argc == 2) ? argv[1] : "");
  net_block_until_connected(net, 20000);
  if (!net->sock_menu) {
    printf("[native] not connected to menu!\n");
    inmenu->has_quit = 1;
  }

  while (!inmenu->has_quit) {
    SDL_Event e;
    if (SDL_WaitEventTimeout(&e, 10)) {
      pads_frame(pads, &e, 0);

      if (net->injected_count)
        ingame_frame(ingame, &e);
      else
        inmenu_frame(inmenu, &e);
    } else {
      // FIXME: make sure this gets called on a certain rate
      // (=> with controller presses all the time, still send the
      // movement bytes at the same rate, don't wait for the event
      // timeout then!!)
      if (net->injected_count)
        ingame_frame(ingame, NULL);
    }

    // check for incoming data
    if (!net_frame(net, (void *)inmenu_recv_callback, (void *)inmenu,
                   (void *)ingame_recv_callback, (void *)ingame))
      break;
  }

  // clean up
  ingame_cleanup(ingame);
  inmenu_cleanup(inmenu);
  net_cleanup(net);
  pads_cleanup(pads);
  cmap_cleanup(cmap);
  SDL_Quit();
  return 0;
}
