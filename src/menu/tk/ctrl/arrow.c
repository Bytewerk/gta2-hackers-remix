#include "../../sty/sprites.h"
#include "../toolkit.h"
#include "controls.h"

tk_el_t *tk_ctrl_arrow(tk_t *tk, tk_el_t *TK_PARENT, char is_left,
                       void *actionfunc, void *userdata) {
  tk_el_t *sprite =
      tk_sprite(tk, TK_PARENT, is_left ? GTA2_SPRITE_ARROW_LEFT_RED
                                       : GTA2_SPRITE_ARROW_RIGHT_RED);
  sprite->actionfunc = actionfunc;
  sprite->userdata = userdata;

  // only display, when selected
  sprite->argb_normal = 0x00ffffff;
  sprite->argb_selected = 0xffffffff;
  return sprite;
}
