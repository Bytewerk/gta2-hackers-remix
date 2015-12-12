#include "../common/cstr/cstr.h"
#include "../common/headers/api_native2menu.h"
#include "../common/headers/common.h"
#include "cmap/cmap.h"
#include "ingame/ingame.h"
#include "inmenu/inmenu.h"
#include "net/native_net.h"
#include "pads/pads.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>

void load_sdl_game_controller_db() {
  char txt[] = "data/SDL_GameControllerDB/gamecontrollerdb.txt";
  printf("loading %s....", txt);
  int ret = SDL_GameControllerAddMappingsFromFile(txt);
  if (ret == -1)
    printf("FAILED: %s (check if the file exists and for syntax"
           " errors!)\n",
           SDL_GetError());
  else
    printf("%i entries found\n", ret);
}

int main(int argc, char **argv) {
  if (!strcmp(SDL_GetPlatform(), "Windows"))
    freopen("native.log", "a", stdout);

  // initialize everything
  if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_TIMER) < 0)
    return printf("ERROR from SDL: %s\n", SDL_GetError());
  if (SDLNet_Init() == -1)
    exit(printf("SDLNet ERROR: %s\n", SDLNet_GetError()));
  load_sdl_game_controller_db();
  cmap_t *cmap = cmap_init();
  pads_t *pads = pads_init(0);
  net_t *net = net_init();
  inmenu_t *inmenu = inmenu_init(net, pads);
  ingame_t *ingame = ingame_init(net, cmap, pads, inmenu);

  // wait up to 7 seconds for the menu connection. If it fails, quit
  net_block_until_connected(net, 7000);
  if (!net->sock_menu) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR, "G2HR",
        "Native: Can't connect to the 'menu' component!\n"
        " GTA2: Hacker's Remix is divided into multiple\n"
        " components, which need to connect to each other over"
        " TCP\nvia localhost in order to do their unholy vodoo"
        " magic.\nCheck your firewall settings!\n\nMore info:"
        " http://git.io/g2hr-firewall",
        NULL);

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
      else
        inmenu_frame_timeout(inmenu);
    }

    // check for incoming data
    if (!net_frame(net, (void *)inmenu_recv_callback, (void *)inmenu,
                   (void *)ingame_recv_callback, (void *)ingame))
      break;
  }

  // copy the 'exec_after_quit' value from the inmenu struct
  char exec_after_quit[G2HR_EXEC_AFTER_QUIT_LEN];
  strncpy(exec_after_quit, inmenu->exec_after_quit,
          G2HR_EXEC_AFTER_QUIT_LEN - 1);

  // clean up
  ingame_cleanup(ingame);
  inmenu_cleanup(inmenu);
  net_cleanup(net);
  pads_cleanup(pads);
  cmap_cleanup(cmap);
  SDL_Quit();
  SDLNet_Quit();

  if (exec_after_quit[0]) {
    printf("[native] executing command after quit:\n\t%s\n", exec_after_quit);
    system(exec_after_quit);
  }

  return 0;
}
