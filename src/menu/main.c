#include "bg/background.h"
#include "sfx/sfx.h"
#include "sty/sty.h"
#include "tk/toolkit.h"
#include "ui/interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const char *tgas[] = {"1",        "1_play",  "1_options",       "1_quit",
                        "2",        "2_name",  "2_league",        "2_level1",

                        "3_tables", "credits", "g2hr_splitscreen"};

  // Initialize everything
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    exit(printf("SDL_ERROR: %s\n", SDL_GetError()));
  IMG_Init(IMG_INIT_PNG);

  sty_t *fsty = sty_load("data/fstyle.sty");

  // FIXME: needs SDL 2.0.4
  sfx_t *sfx = sfx_init();
  bg_t *bg = bg_init(tgas, sizeof(tgas) / sizeof(char *));
  tk_t *tk = tk_init(fsty, sfx, bg, "G2HR");
  ui_t *ui = ui_init(tk);

  while (!tk->quit) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      break;
    tk_frame(tk, &event);
  }

  ui_cleanup(ui);
  tk_cleanup(tk);
  bg_cleanup(bg);
  sty_cleanup(fsty);
  sfx_cleanup(sfx);
  IMG_Quit();
  SDL_Quit();

  printf("cleaned up!\n");
  return 0;
}
