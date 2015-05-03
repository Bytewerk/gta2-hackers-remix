#include "../../tk/ctrl/controls.h"
#include "../../tk/toolkit.h"
#include "../interface.h"
#include "../ui_text.h"

/*
        TODO:
                - replace all the buttons with functional controls
                - store 'int controllers_found' in the client struct
                        (communication between native and menu)
                - parse the screen layout config and send the abstracted
                        geometry values to the meta component instead of the
                        layout ID (this is for debugging only)
                - maybe put the time values in a config, too
*/

// TODO: place this in a common.h or something like that
#define GTA2_MP_MAX_PLAYERS 6

#define TODO_controllers_found 4
#define TODO_screen_layout_max ('F' - 'A')

// USERDATA STRUCT
typedef struct {
  ui_t *ui;

  tk_el_t *titlebar;
  tk_el_t *players;
  tk_el_t *screen_layout;
  tk_el_t *map;
  tk_el_t *game_type;
  tk_el_t *time;
  tk_el_t *cops;
  tk_el_t *play;
} ud_splitscreen_t;

// ACTIONFUNC
void splitscreen_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                            tk_action_t action, SDL_Keycode key) {
  ud_splitscreen_t *ud = (ud_splitscreen_t *)el->userdata;
  if (action == TK_ACTION_ENTER && el_selected == ud->play) {
    int players =
        1 + ((ud_arrowtext_t *)(ud->players->userdata))->entry_selected;

    int screen_layout =
        ((ud_arrowtext_t *)(ud->screen_layout->userdata))->entry_selected;

    char *time =
        ud->ui->multiplayer_time_values
            ->values[((ud_arrowtext_t *)(ud->time->userdata))->entry_selected];

    char game_type =
        ((ud_arrowtext_t *)ud->game_type->userdata)->entry_selected;

    int cops_enabled = ((ud_arrowtext_t *)ud->cops->userdata)->entry_selected;

    char *buffer = malloc(100);
    snprintf(buffer, 100, "SPLITSCREEN %i %i %s %i %i", players, screen_layout,
             time, game_type, cops_enabled);
    server_send(ud->ui->server, buffer, 1);
  }
}

// SCREEN INIT & LAYOUT

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui) {
  ud_splitscreen_t *ud = malloc(sizeof(ud_splitscreen_t));
  ud->ui = ui;

  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);

  // create the screen layout
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

          // players
          ud->players =
              tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, 0, ui->numbers,
                                TODO_controllers_found, "PLAYERS: ", NULL, NULL,
                                NULL, NULL, NULL /*bottom text*/);

          // screen layout
          ud->screen_layout =
              tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, 0, ui->letters,
                                TODO_screen_layout_max, "SCREEN LAYOUT: ", NULL,
                                NULL, NULL, NULL, NULL /*bottom text*/);

          // map (FIXME: change screen on enter)
          ud->map = tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, 0, ui->maps,
                                      ui->mmp->file_count, "MAP: ", NULL,
                                      "ENTER: SHOW A LIST", NULL, NULL, NULL);

          // game type
          ud->game_type =
              tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, 0, ui->game_types,
                                G2HR_UI_GAME_TYPES_COUNT, "GAME TYPE: ", NULL,
                                NULL, NULL, NULL, NULL /*bottom text*/);

          // time
          ud->time =
              tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, 0,
                                ui->multiplayer_time_values->values,
                                ui->multiplayer_time_values->count, "TIME: ",
                                " MIN", NULL, NULL, NULL, NULL /*bottom text*/);

          // cops
          ud->cops = tk_ctrl_boolean(tk, TK_PARENT, NULL, "COPS: ");

          // big play button. We don't have the huge font in red
          // for black backgrounds, but modifying argb_selected has
          // the same effect :)

          ud->play = tk_ctrl_button(tk, TK_PARENT, "PLAY!", NULL, NULL);
          ud->play->font_id = ud->play->font_id_selected =
              GTA2_FONT_FSTYLE_WHITE_BLACK_HUGE;
          ud->play->argb_selected = 0xffff0000;
          tk_el_padding(ud->play, 0, 20, 0, 0);

          );

      );
  return splitscreen;
}
