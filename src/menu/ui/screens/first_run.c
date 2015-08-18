#include "../../../common/headers/common.h"
#include "../../../common/ini/ini.h"
#include "../../chk/chk.h"
#include "../../tk/ctrl/controls.h"
#include "../ui.h"

typedef struct {
  ui_t *ui;
  tk_el_t *yes;
  tk_el_t *no;
  tk_el_t *quit;

} ud_first_run_t;

#define IS_NOT_FIRST_RUN                                                       \
  strcmp(ini_read(ud->ui->ini_settings, "ui", "update_check_enabled"), "ask")

void first_run_actionfunc(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                          tk_action_t action) {
  ud_first_run_t *ud = (ud_first_run_t *)el->userdata;

  // clear the red bar at the top and the DMA and Rockstar logos at
  // the bottom of the background
  if (action == TK_ACTION_DRAW_BEFORE_CONTROLS) {
    SDL_Renderer *renderer = ud->ui->tk->renderer;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

    SDL_Rect r = {0, 0, 640, 45};
    SDL_RenderFillRect(renderer, &r);
    SDL_Rect r2 = {0, 400, 640, 80};
    SDL_RenderFillRect(renderer, &r2);
    ud->quit->text = IS_NOT_FIRST_RUN ? "BACK" : "QUIT";
  }

  // quit
  if (action == TK_ACTION_BACKSPACE || action == TK_ACTION_ESC ||
      (action == TK_ACTION_ENTER && el_selected == ud->quit)) {
    if (IS_NOT_FIRST_RUN)
      tk->screen_active = ud->ui->options;
    else
      tk->quit = 1;
  }

  if (action != TK_ACTION_ENTER)
    return;

  if (el_selected == ud->yes || el_selected == ud->no) {
    tk->screen_active = IS_NOT_FIRST_RUN ? ud->ui->options : ud->ui->main;
  }

  if (el_selected == ud->yes) {
    ini_modify(ud->ui->ini_settings, "ui", "update_check_enabled", "true",
               true);
    ud->ui->chk = chk_init(ud->ui->tk->pref_path, true);
    ini_save(ud->ui->ini_settings, NULL, false, false);
  } else if (el_selected == ud->no) {
    ini_modify(ud->ui->ini_settings, "ui", "update_check_enabled", "false",
               true);
    ini_save(ud->ui->ini_settings, NULL, false, false);
  }
}

#define ADD_LINE(TEXT)                                                         \
  tk_label(tk, TK_PARENT, TEXT, GTA2_FONT_FSTYLE_WHITE_BLACK_TINY, 0)

tk_screen_t *ui_screen_first_run(tk_t *tk, ui_t *ui) {
  tk_screen_t *first_run = tk_screen(tk, NULL, NULL);

  ud_first_run_t *ud = malloc(sizeof(ud_first_run_t));
  ud->ui = ui;

  TK_STACK_SCREEN(
      first_run,

      TK_PARENT->bg_mashup =
          bg_mashup(tk->bg, "levelcomplete", NULL, NULL, NULL);

      tk_el_padding(TK_PARENT, 20, 150, 0, 0); tk_el_width(TK_PARENT, 600);

      tk_el_t *hr_text =
          tk_label(tk, TK_PARENT, "HACKER'S REMIX V" G2HR_VERSION,
                   GTA2_FONT_FSTYLE_WHITE_BLACK_NORMAL, 0);
      tk_el_padding(hr_text, 0, 0, 0, 40); tk_el_center(hr_text);

      ADD_LINE(
          "Welcome and thanks for running the 0.4 release of this meta-mod!");
      ADD_LINE("It is a technical preview that probably has lots of bugs, so "
               "please");
      ADD_LINE(
          "contribute patches if you have the skillz at g2hr.org (github) :)");
      TK_FLOW(tk_el_padding(TK_PARENT, 0, 20, 0, 0);
              ADD_LINE("Do you wish to enable ");
              ADD_LINE("daily update checks")->argb_normal =
                  GTA2_FONT_COLOR_RED;
              ADD_LINE(" when the game runs?");)
          ADD_LINE("(Make a selection with your keyboard or game controller)");

      TK_STACK(first_run->el_content_container = TK_PARENT;
               tk_el_padding(TK_PARENT, 280, 50, 0, 0);
               TK_PARENT->actionfunc = first_run_actionfunc;
               TK_PARENT->userdata = (void *)ud;

               ud->yes = tk_ctrl_button(tk, TK_PARENT, "YES", NULL, NULL);
               ud->no = tk_ctrl_button(tk, TK_PARENT, "NO", NULL, NULL);
               tk_el_padding(ud->no, 7, 0, 0, 0);

               ud->quit = tk_ctrl_button(tk, TK_PARENT, "QUIT", NULL, NULL);
               tk_el_padding(ud->quit, 0, 20, 0, 0);));

  return first_run;
}

#undef ADD_LINE
