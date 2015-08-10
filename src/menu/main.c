#include "../common/cstr/cstr.h"
#include "bg/background.h"
#include "gxt/gxt.h"
#include "mmp/mmp.h"
#include "net/menu_net.h"
#include "rpc/rpc.h"
#include "sfx/sfx.h"
#include "sl/sl.h"
#include "sty/sty.h"
#include "tk/toolkit.h"
#include "ui/ui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define G2HR_MENU_FPS 20

/*
        NOTE: the net sub-component connects to the native component on
                startup. When it fails for 4 seconds, it shows a message box
                and quits.
*/

int main(int argc, char *argv[]) {
  if (!getenv("WINEPREFIX"))
    freopen("menu.log", "a", stdout);

  // initialize the random seed
  srand((unsigned int)time(NULL));

  // FIXME: attach gxt to ui instead of tk!

  // init all
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    exit(printf("SDL ERROR: %s\n", SDL_GetError()));
  if (SDLNet_Init() == -1)
    exit(printf("SDLNet ERROR: %s\n", SDLNet_GetError()));
  IMG_Init(IMG_INIT_PNG);
  net_t *net = net_init();

  // load configs
  char *pref_path = SDL_GetPrefPath("bytewerk", "G2HR");
  sl_t *sl = sl_init("data/screen_layouts.cfg");

  // load GTA2 files
  gxt_t *gxt = gxt_load("GTA2/data/e.gxt");
  sty_t *fsty = sty_load("GTA2/data/fstyle.sty");
  sfx_t *sfx = sfx_init();
  mmp_t *mmp = mmp_init("GTA2/data");
  bg_t *bg = bg_init();

  tk_t *tk = tk_init(gxt, fsty, sfx, bg, pref_path, "GTA2: HACKER'S REMIX");
  ui_t *ui = ui_init(tk, mmp, net, sl);

  // run registry path changer
  rpc_init(pref_path);

  // main loop
  uint32_t previous = 0;
  while (!tk->quit) {
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, 100 / G2HR_MENU_FPS)) {
      if (event.type == SDL_QUIT)
        break;
      tk_frame(tk, &event);
    } else {
      uint32_t now = SDL_GetTicks();
      if (now - previous >= 1000 / G2HR_MENU_FPS) {
        previous = now;
        tk_frame(tk, NULL);
      }
    }

    if (!net_frame(net))
      tk->quit = 1;
  }

  // cleanup all
  ui_cleanup(ui);
  tk_cleanup(tk);
  sl_cleanup(sl);
  free(pref_path);
  mmp_cleanup(mmp);
  bg_cleanup(bg);
  sfx_cleanup(sfx);
  sty_cleanup(fsty);
  gxt_cleanup(gxt);
  net_cleanup(net);
  IMG_Quit();
  SDLNet_Quit();
  SDL_Quit();
  printf("cleaned up!\n");

  return 0;
}
