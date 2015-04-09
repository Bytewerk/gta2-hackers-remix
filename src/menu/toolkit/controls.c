#include "control_data.h"
#include "toolkit.h"

void tk_control_setbg(tk_t *tk, tk_control_t *ctrl, const char *full,
                      const char *left, const char *right) {
  if (ctrl->bg)
    free(ctrl->bg);
  ctrl->bg = tk_create_background(tk, full, left, right);
}

// always use the tk_control_add macro instead!
tk_control_t *tk_control_add_internal(tk_screen_t *screen, const char *title,
                                      char type, void *data) {
  tk_control_t *ctrl = malloc(sizeof(tk_control_t));

  ctrl->next = NULL;
  ctrl->title = title;
  ctrl->type = type;
  ctrl->data = data;
  ctrl->bg = NULL;
  ctrl->bottom_text_high = NULL;
  ctrl->bottom_text_low = NULL;
  ctrl->disabled = 0;

  if (screen->first_control) {
    tk_control_t *listpos = screen->first_control;
    while (listpos->next)
      listpos = listpos->next;
    listpos->next = ctrl;
  } else {
    screen->first_control = ctrl;
    screen->selected_control = ctrl;
  }
  return ctrl;
}

tk_control_t *tk_control_cleanup(tk_control_t *ctrl) {
  tk_control_t *todo = ctrl->next;
  free(ctrl->data); // is always set!
  if (ctrl->bg)
    free(ctrl->bg);
  free(ctrl);
  return todo;
}

int tk_control_get_height(tk_control_t *ctrl) {
  switch (ctrl->type) {
  case TK_BUTTON:
    return 12;
  case TK_CIRCLE:
    return 12;
  }
  return 20;
}

int tk_control_get_vpadding(tk_control_t *ctrl) {
  switch (ctrl->type) {
  case TK_BUTTON:
    return 8;
  case TK_CIRCLE:
    return 8 + 25 + 8;
  }
  return 0;
}

/*
        NOTE: Always have at least one enabled control on a screen,
                otherwise the up/down functions will loop until stack overflow
                (not the website ;) ).
*/

void tk_control_up(tk_screen_t *screen) {
  tk_control_t *selected = screen->selected_control;
  tk_control_t *listpos = screen->first_control;
  if (!selected)
    return;

  // first entry -> get the last one!
  if (selected == listpos)
    while (listpos->next)
      listpos = listpos->next;
  else
    while (listpos && listpos->next != selected)
      listpos = listpos->next;

  screen->selected_control = listpos;

  if (screen->selected_control->disabled)
    tk_control_up(screen);
}

void tk_control_down(tk_screen_t *screen) {
  if (!screen->selected_control)
    return;

  screen->selected_control = screen->selected_control->next
                                 ? screen->selected_control->next
                                 : screen->first_control;

  if (screen->selected_control->disabled)
    tk_control_down(screen);
}

void tk_control_mouse(tk_screen_t *screen, int x, int y, char /*bool*/ click) {
  tk_control_t *listpos = screen->first_control;
  int offset_y = 0; // set to screen's control y-offset
  while (listpos) {
    /*
            check if x,y inside of control area
            otherwise offset_y+= control height;
    */
    listpos = listpos->next;
  }
}

void tk_control_onclick(tk_t *tk) {
  if (!tk->screen)
    return;

  tk_control_t *ctrl = tk->screen->selected_control;
  if (!ctrl)
    return;

  if (tk->screen->event_func) {
    void (*event_func)(tk_t *, void *) = tk->screen->event_func;

    event_func(tk, tk->screen->ui_data);
  }

  if (ctrl->type == TK_BUTTON) {
    TK_BUTTON_DATA_t *data = (TK_BUTTON_DATA_t *)ctrl->data;
    // if(data->onclick_func) -- TODO

    if (data->onclick_screen)
      tk->screen = data->onclick_screen;
  }
}
