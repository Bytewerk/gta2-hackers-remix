#include "sty.h"
#include <stdio.h>

/*
        TODO:
                - resize the letters, according to the resolution
                - add automatic line breaks (incl. max_width parameter)?
                - cache whole font sets as textures before using them?
                        (not sure if this has any benefit)

        FIXME: dest.w, dest.h gets ignored

        This function draws a text string directly to the renderer.
        Text must be \0-terminated!
*/

#define GTA2_FIRST_CHAR '!'

void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, SDL_Rect dest,
              char *text) {
  int base =
      sty->sprite_base.font + sty->font_base.base[font_id] - GTA2_FIRST_CHAR;

  if (font_id >= sty->font_base.font_count) {
    printf("WARNING: Can't draw font_id %i (max: %i, text: %s)!\n", font_id,
           sty->font_base.font_count - 1, text);
    return;
  }

  for (; *text != '\0'; text++) {
    char letter = *text;

    // There is no space 'letter' in the font, so just use half
    // the width of the 'A' character. Inspired by Black_Phoenix'
    // blog post: http://brain.wireos.com/?p=1647
    if (letter == ' ') {
      dest.x += sty->sprite_index.entries['A' + base].width / 2;
      continue;
    }

    int width, height;

    SDL_Texture *sprite = sty_sprite(renderer, sty, 0, letter + base);
    SDL_QueryTexture(sprite, NULL, NULL, &width, &height);

    SDL_Rect letter_dest = {dest.x, dest.y, width, height};
    SDL_RenderCopy(renderer, sprite, NULL, &letter_dest);

    dest.x += width;
  }
}
