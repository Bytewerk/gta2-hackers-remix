#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

/*
        TODO:
                - replace all the buttons with functional controls
                - store 'int controllers_found' in the client struct
                        (communication between native and menu)
                - parse the screen layout config
                - maybe put the time values in a config, too
*/

typedef struct {
  tk_el_t *titlebar;
  tk_el_t *players;
  tk_el_t *screen_layout;
  tk_el_t *map;
  tk_el_t *game_type;
  tk_el_t *time;
  tk_el_t *cops;
  tk_el_t *play;

} ud_splitscreen_t;

#define TODO_controllers_found 6

void splitscreen_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                            tk_action_t action, SDL_Keycode key) {
  ud_splitscreen_t *ud = (ud_splitscreen_t *)el->userdata;
  if (action == TK_ACTION_ENTER && el_selected == ud->play) {
    printf("STUB: PLAY!\n");
  }
}
#undef TODO_controllers_found

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui, tk_screen_t *levels) {
  ud_splitscreen_t *ud = malloc(sizeof(ud_splitscreen_t));
  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);

  TK_STACK_SCREEN(
      splitscreen,

      TK_PARENT->bg_mashup =
          bg_mashup(tk->bg, "3_tables", NULL, NULL, "g2hr_splitscreen");

      // title bar
      ud->titlebar = tk_label(tk, TK_PARENT, "4 CONTROLLERS FOUND",
                              GTA2_FONT_FSTYLE_WHITE_RED_NORMAL, 0);
      tk_el_padding(ud->titlebar, 315, 12, 0, 0);

      // content
      TK_STACK(
          splitscreen->el_content_container = TK_PARENT;
          tk_el_padding(TK_PARENT, 300, 170, 0, 0); TK_PARENT->userdata = ud;
          TK_PARENT->actionfunc = (void *)splitscreen_actionfunc;

          ud->players =
              tk_ctrl_button(tk, TK_PARENT, "PLAYERS: 3", NULL, NULL, NULL);

          ud->screen_layout = tk_ctrl_button(tk, TK_PARENT, "SCREEN LAYOUT: A",
                                             NULL, NULL, NULL);

          ud->map = tk_ctrl_button(tk, TK_PARENT, "MAP: TINY TOWN", NULL,
                                   levels, NULL);

          ud->game_type = tk_ctrl_button(tk, TK_PARENT, "GAME TYPE: FRAGS",
                                         NULL, NULL, NULL);

          ud->time =
              tk_ctrl_button(tk, TK_PARENT, "TIME: 5 MIN", NULL, NULL, NULL);

          ud->cops = tk_ctrl_boolean(tk, TK_PARENT, "COPS", NULL, 1);

          // big play button. We don't have the huge font in red
          // for black backgrounds, but modifying argb_selected has
          // the same effect :)

          ud->play = tk_ctrl_button(tk, TK_PARENT, "PLAY!", NULL, NULL, NULL);
          ud->play->font_id = ud->play->font_id_selected =
              GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE;
          ud->play->argb_selected = 0xffff0000;
          tk_el_padding(ud->play, 0, 20, 0, 0);

          );

      );
  return splitscreen;
}
