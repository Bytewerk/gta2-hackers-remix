#include "bg/background.h"
#include "gxt/gxt.h"
#include "mmp/mmp.h"
#include "server/server.h"
#include "sfx/sfx.h"
#include "sty/sty.h"
#include "tk/toolkit.h"
#include "ui/interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <stdlib.h>

#define G2HR_MENU_FPS 20

int main(int argc, char *argv[]) {
  const char *pics[] = {"1",        "1_play",   "1_options",       "1_quit",
                        "2",        "2_name",   "2_league",        "2_level1",
                        "2_level2", "2_level3", "2_bonus1",        "2_bonus2",
                        "2_bonus3",

                        "3_tables", "credits",  "g2hr_splitscreen"};

  // FIXME: attach gxt to ui instead of tk!

  // init all
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    exit(printf("SDL ERROR: %s\n", SDL_GetError()));
  if (SDLNet_Init() == -1)
    exit(printf("SDLNet ERROR: %s\n", SDLNet_GetError()));
  IMG_Init(IMG_INIT_PNG);
  server_t *server = server_init();
  gxt_t *gxt = gxt_load("GTA2/data/e.gxt");
  sty_t *fsty = sty_load("GTA2/data/fstyle.sty");
  sfx_t *sfx = sfx_init();
  mmp_t *mmp = mmp_init("GTA2/data");
  bg_t *bg = bg_init(pics, sizeof(pics) / sizeof(char *));
  tk_t *tk = tk_init(gxt, fsty, sfx, bg, "G2HR");
  ui_t *ui = ui_init(tk, mmp, server);

  // start the meta component (this will simply fail on Linux)
  char *buffer = malloc(100);
  snprintf(buffer, 100, "start bin/meta.exe %i", server->port);
  system(buffer);
  free(buffer);

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

    server_frame(server);
  }

  // cleanup all
  ui_cleanup(ui);
  tk_cleanup(tk);
  mmp_cleanup(mmp);
  bg_cleanup(bg);
  sfx_cleanup(sfx);
  sty_cleanup(fsty);
  gxt_cleanup(gxt);
  server_cleanup(server);
  IMG_Quit();
  SDLNet_Quit();
  SDL_Quit();

  printf("cleaned up!\n");
  return 0;
}
