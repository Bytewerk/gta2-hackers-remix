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

// CONTROL VALUES
#define GTA2_CTRL_PLAYERS_TEMPLATE "PLAYERS: N"
char **screen_players_values() {
  char **ret = malloc(sizeof(char *) * GTA2_MP_MAX_PLAYERS);
  for (int i = 0; i < GTA2_MP_MAX_PLAYERS; i++) {
    ret[i] = malloc(sizeof(GTA2_CTRL_PLAYERS_TEMPLATE));
    strncpy(ret[i], GTA2_CTRL_PLAYERS_TEMPLATE,
            sizeof(GTA2_CTRL_PLAYERS_TEMPLATE));
    ret[i][9] = '1' + i;
  }
  return ret;
}

#define TODO_screen_layout_max ('F' - 'A')
#define GTA2_CTRL_SCREEN_LAYOUT_TEMPLATE "SCREEN LAYOUT: N"
char **screen_layout_values() {
  char **ret = malloc(sizeof(char *) * TODO_screen_layout_max);
  for (int i = 0; i < TODO_screen_layout_max; i++) {
    ret[i] = malloc(sizeof(GTA2_CTRL_SCREEN_LAYOUT_TEMPLATE));
    strncpy(ret[i], GTA2_CTRL_SCREEN_LAYOUT_TEMPLATE,
            sizeof(GTA2_CTRL_SCREEN_LAYOUT_TEMPLATE));
    ret[i][sizeof(GTA2_CTRL_SCREEN_LAYOUT_TEMPLATE) - 2] = 'A' + i;
  }
  return ret;
}

#define GTA2_CTRL_TIME_PREFIX "TIME: "
#define GTA2_CTRL_TIME_SUFFIX " MIN"
char **screen_time_values(cfg_split_t *times) {
  int count = times->count;
  char **ret = malloc(sizeof(char *) * count);
  size_t prefix_len = sizeof(GTA2_CTRL_TIME_PREFIX) - 1;
  size_t suffix_len = sizeof(GTA2_CTRL_TIME_SUFFIX) - 1;
  for (int i = 0; i < count; i++) {
    char *time = times->values[i];
    size_t len = strlen(time);
    size_t total_length = prefix_len + len + suffix_len + 1;
    ret[i] = malloc(total_length);
    snprintf(ret[i], total_length, "%s%s%s", GTA2_CTRL_TIME_PREFIX, time,
             GTA2_CTRL_TIME_SUFFIX);
  }
  return ret;
}

// USERDATA STRUCT
typedef struct {
  ui_t *ui;
  char **players_values;
  char **screen_layout_values;
  char **time_values;

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
#define TODO_controllers_found 6

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

    char *buffer = malloc(100);
    snprintf(buffer, 100, "SPLITSCREEN %i %i %s", players, screen_layout, time);
    server_send(ud->ui->server, buffer, 1);
  }

  if (action == TK_ACTION_CLEANUP) {
    for (int i = 0; i < GTA2_MP_MAX_PLAYERS; i++)
      free(ud->players_values[i]);
    free(ud->players_values);

    for (int i = 0; i < TODO_screen_layout_max; i++)
      free(ud->screen_layout_values[i]);
    free(ud->screen_layout_values);

    for (int i = 0; i < ud->ui->multiplayer_time_values->count; i++)
      free(ud->time_values[i]);
    free(ud->time_values);
  }
}

// SCREEN INIT & LAYOUT

tk_screen_t *ui_screen_splitscreen(tk_t *tk, ui_t *ui, tk_screen_t *levels) {
  ud_splitscreen_t *ud = malloc(sizeof(ud_splitscreen_t));
  ud->ui = ui;

  tk_screen_t *splitscreen = tk_screen(tk, NULL, NULL);

  // generate the control values
  ud->players_values = screen_players_values();
  ud->screen_layout_values = screen_layout_values();
  ud->time_values = screen_time_values(ui->multiplayer_time_values);

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
          ud->players = tk_ctrl_arrowtext(
              tk, TK_PARENT, NULL /*bg*/, ud->players_values,
              TODO_controllers_found, 1, 0, NULL, 1, NULL, NULL, NULL, NULL);

          // screen layout
          ud->screen_layout = tk_ctrl_arrowtext(
              tk, TK_PARENT, NULL /*bg*/, ud->screen_layout_values,
              TODO_screen_layout_max, 1, 0, NULL, 1, NULL, NULL, NULL, NULL);

          // map (FIXME)
          ud->map = tk_ctrl_button(tk, TK_PARENT, "MAP: TINY TOWN", NULL,
                                   levels, NULL);
          ud->map->bottom_text_low = "ENTER: CHOOSE A MAP";

          // game type
          ud->game_type = tk_ctrl_button(tk, TK_PARENT, "GAME TYPE: FRAGS",
                                         NULL, NULL, NULL);

          // time
          ud->time =
              tk_ctrl_arrowtext(tk, TK_PARENT, NULL /*bg*/, ud->time_values,
                                ui->multiplayer_time_values->count, 1, 0, NULL,
                                1, NULL, NULL, NULL, NULL);

          // cops
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
