#include "interface.h"
#include "../tk/toolkit.h"

ui_t *ui_init(tk_t *tk) {
  ui_t *ui = malloc(sizeof(ui_t));
  ui->tk = tk;

  // MAIN MENU
  tk_screen_t *main_menu = tk_screen(tk);

  TK_STACK_SCREEN(main_menu, tk_el_padding(TK_PARENT, 300, 250, 0, 0);

                  );

  // PLAY
  tk_screen_t *play = tk_screen(tk);

  TK_STACK_SCREEN(play, tk_el_padding(TK_PARENT, 300, 210, 0, 0););

  /*
  // Credits screen (TODO: actually display credits ;) )
  ui->credits = tk_screen_create(tk, (void*) ui, CREDITS,
          NULL, &ui_quit);
  tk_screen_setbg(tk, ui->credits, "credits", NULL, NULL);


  // Play screen
  ui->play = tk_screen_create(tk, (void*) ui, NORMAL,NULL,NULL);

  tk_control_t* name;
  tk_control_add(ui->play, name, "PLAYER 0", TK_BUTTON, NULL);
  tk_control_setbg(tk, name, NULL, "2_name", "2");
  name->bottom_text_high = "ENTER:  EDIT NAME";
  name->bottom_text_low  = "DELETE:  DELETE PLAYER";


  tk_control_t* resume;
  tk_control_add(ui->play, resume, "RESUME SAVED STATUS", TK_BUTTON, NULL);
  resume->disabled = 1;

  tk_control_t* scores;
  tk_control_add(ui->play, scores, "VIEW HIGH SCORES", TK_BUTTON, NULL);
  tk_control_setbg(tk, scores, NULL, "2_league", "2");

  tk_control_t* start;
  tk_control_add(ui->play, start, "START PLAY IN AREA", TK_CIRCLE,
          data->value = '1';
  );
  tk_control_setbg(tk, start, NULL, "2_level1", "2");



  // Main menu (TODO: save the version number as #define somewhere)
ui->main = tk_screen_create(tk, (void*) ui, BOTTOM_RIGHT,
          ui->credits, NULL);
  ui->main->bottom_text_low = "G2HR V0.4";

tk_control_t* play;
tk_control_add(ui->main, play, UI_TEXT_PLAY, TK_BUTTON,
          data->onclick_screen = ui->play);
tk_control_setbg(tk, play, NULL, "1_play", "1");

tk_control_t* options;
tk_control_add(ui->main, options, UI_TEXT_OPTIONS, TK_BUTTON, NULL);
tk_control_setbg(tk, options, NULL, "1_options", "1");

tk_control_t* quit;
tk_control_add(ui->main, quit, UI_TEXT_QUIT, TK_BUTTON,
          data->onclick_screen = ui->credits);
tk_control_setbg(tk, quit, NULL, "1_quit", "1");



// Now that the main menu has been created, set 'back' links
ui->play->back = ui->main;

// Default screen is Main Menu
tk->screen = ui->main;
*/

  return ui;
}

void ui_cleanup(ui_t *ui) { free(ui); }
