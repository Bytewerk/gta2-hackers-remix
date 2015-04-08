#include "font.h"
#include "sty.h"
#include <stdio.h>

// only dest.x and dest.y get used
void sty_circle(SDL_Renderer *renderer, sty_t *sty, SDL_Rect dest,
                char letter) {
  SDL_Texture *sprite;
  dest.x += 105;
  dest.y += 25;
  dest.w = dest.h = 32;

  // circle
  sprite = sty_sprite(renderer, sty, 0, 4);
  SDL_RenderCopy(renderer, sprite, NULL, &dest);
  SDL_DestroyTexture(sprite);

  dest.x += 13;
  dest.y += 4;

  // letter
  int sprite_id = letter + sty->sprite_base.font +
                  sty->font_base.base[GTA2_FONT_FSTYLE_WHITE_RED_NORMAL] -
                  GTA2_FONT_FIRST_CHAR;

  sprite = sty_sprite(renderer, sty, 0, sprite_id);
  SDL_QueryTexture(sprite, NULL, NULL, &dest.w, &dest.h);
  SDL_RenderCopy(renderer, sprite, NULL, &dest);
  SDL_DestroyTexture(sprite);
}
