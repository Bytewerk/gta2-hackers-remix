#include "font.h"
#include "sty.h"
#include <stdio.h>

/*
        TODO:
                - add color/opacity parameter wrapper?
                - add automatic line breaks (incl. max_width parameter)?
                        (only if we ever need multiline text)
                - cache whole font sets as textures before using them?
                        (not sure if this has any benefit)

        FIXME: dest.w, dest.h gets ignored

        This function draws a text string directly to the renderer.
        Text must be \0-terminated!
*/

void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, SDL_Rect dest,
              const char *text) {
  int base = sty->sprite_base.font + sty->font_base.base[font_id] -
             GTA2_FONT_FIRST_CHAR;

  if (font_id >= sty->font_base.font_count) {
    printf("WARNING: Can't draw font_id %i (max: %i, text: %s)!\n", font_id,
           sty->font_base.font_count - 1, text);
    return;
  }

  for (; *text != '\0'; text++) {
    char letter = *text;

    // some letters are switched
    if (letter == ';')
      letter = ':';
    if (letter == ':')
      letter = ';';

    if (letter == ' ') {
      // Pixel perfect for GTA2_FONT_FSTYLE_WHITE_BLACK_TINY
      dest.x += 8;
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
