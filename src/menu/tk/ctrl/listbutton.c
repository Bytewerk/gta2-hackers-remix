#include "../../sty/font.h"
#include "../../sty/sprites.h"
#include "../toolkit.h"
#include "controls.h"

tk_el_t *tk_ctrl_listbutton(tk_t *tk, tk_el_t *TK_PARENT,
                            bg_mashup_t *bg_mashup, char **entries,
                            uint16_t entry_count, uint16_t entry_selected,
                            char entry_length, char *title,
                            char editing_disabled, char *bottom_text_high,
                            char *bottom_text_low,
                            char *bottom_text_high_editing,
                            char *bottom_text_low_editing) {
  tk_el_t *container;
  TK_STACK(
      container = TK_PARENT; container->bg_mashup = bg_mashup;
      tk_el_selectable(container);

      tk_el_t *label = tk_label(tk, TK_PARENT, title);
      label->font_id = GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL;
      label->font_id_selected = GTA2_FONT_FSTYLE_RED_BLACK_NORMAL;

      tk_el_t *arrowtext = tk_ctrl_arrowtext(
          tk, TK_PARENT, NULL, entries, entry_count, entry_selected,
          entry_length, NULL, editing_disabled, bottom_text_high,
          bottom_text_low, bottom_text_high_editing, bottom_text_low_editing);

      tk_el_padding(arrowtext, 80, 0, 0, 10); tk_el_unselectable(arrowtext););

  return container;
}
