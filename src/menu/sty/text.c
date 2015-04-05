#include "sty.h"
#include <stdio.h>

/*
        TODO:
                - add automatic line breaks (incl. max_width parameter)
                - cache whole font sets as textures before using them?
                - add x,y offsets
                - resize the letters, according to the resolution

        This function draws a text string directly to the renderer.
        Text must be \0-terminated!
*/

void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, char *text) {
  int offset_x = 0;
  int offset_y = 0;

  for (; *text != '\0'; text++) {
    char letter = *text;
    int width, height;
    int sprite_id =
        (letter - 'a') + sty->sprite_base.font + sty->font_base.base[font_id];

    SDL_Texture *sprite = sty_sprite(renderer, sty, sprite_id);
    SDL_QueryTexture(sprite, NULL, NULL, &width, &height);

    SDL_Rect dest = {offset_x, offset_y, width, height};
    SDL_RenderCopy(renderer, sprite, NULL, &dest);

    offset_x += width;
  }
}
