#include "font.h"
#include "sty.h"
#include <stdio.h>

/*
        TODO:
                - add automatic line breaks (incl. max_width parameter)?
                        (only if we ever need multiline text)
                - cache whole font sets as textures before using them?
                        (not sure if this has any benefit)

        FIXME: dest.w, dest.h gets ignored

        This function draws a text string directly to the renderer.


        text: must be \0-terminated!

        argb: set to 0xffffffff, if you don't want to change the colors or
                transparency. Make sure you put the right amount of 'f's there
                ;)
*/

char sty_font_spacing(int font_id) {
  switch (font_id) {
  case GTA2_FONT_FSTYLE_WHITE_BLACK_TINY:
    return 8;
  case GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL:
    return 13;
  case GTA2_FONT_FSTYLE_RED_BLACK_NORMAL:
    return 13;
  case GTA2_FONT_FSTYLE_WHITE_RED_NORMAL:
    return 13;
  }
  return 0;
}

// some letters are switched
char sty_letter_switch(char letter) {
  if (letter == ';')
    return ':';
  if (letter == ':')
    return ';';
  return letter;
}

// just calculate the width and height without actually rendering
void sty_text_measure(sty_t *sty, int *width, int *height, int font_id,
                      const char *text) {
  int base = sty->sprite_base.font + sty->font_base.base[font_id] -
             GTA2_FONT_FIRST_CHAR;

  *height = sty->sprite_index.entries[base + GTA2_FONT_FIRST_CHAR].height;
  *width = 0;
  for (; *text != '\0'; text++) {
    char letter = sty_letter_switch(*text);
    if (letter == ' ')
      *width += sty_font_spacing(font_id);
    else
      *width += sty->sprite_index.entries[letter + base].width;
  }
}

// returns the text width after rendering
void sty_text(SDL_Renderer *renderer, sty_t *sty, int font_id, uint32_t argb,
              int offset_x, int offset_y, const char *text) {
  int base = sty->sprite_base.font + sty->font_base.base[font_id] -
             GTA2_FONT_FIRST_CHAR;
  int width = 0;

  if (font_id >= sty->font_base.font_count) {
    printf("WARNING: Can't draw font_id %i (max: %i, text: %s)!\n", font_id,
           sty->font_base.font_count - 1, text);
    return;
  }

  for (; *text != '\0'; text++) {
    char letter = sty_letter_switch(*text);

    if (letter == ' ') {
      width += sty_font_spacing(font_id);
      continue;
    }

    int width_letter, height_letter;
    int sprite_id = letter + base;
    sty_sprite_measure(sty, &width_letter, &height_letter, sprite_id);

    sty_sprite_draw(renderer, sty, sprite_id, offset_x + width, offset_y,
                    width_letter, height_letter, argb);

    width += width_letter;
  }
}
