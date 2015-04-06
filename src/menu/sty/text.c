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

        FIXME: offset doesn't really work yet!
*/

#define GTA2_FIRST_CHAR '!'

void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, char *text) {
  int offset_x = 0;
  int offset_y = 0;
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
      offset_x += sty->sprite_index.entries['A' + base].width / 2;
      continue;
    }

    int width, height;

    SDL_Texture *sprite = sty_sprite(renderer, sty, 0, letter + base);
    SDL_QueryTexture(sprite, NULL, NULL, &width, &height);

    SDL_Rect dest = {offset_x, offset_y, width, height};
    SDL_RenderCopy(renderer, sprite, NULL, &dest);

    offset_x += width;
  }
}
