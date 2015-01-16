#include "control_data.h"
#include "toolkit.h"

void tk_control_setbg(tk_t *tk, tk_control_t *ctrl, const char *full,
                      const char *left, const char *right) {
  if (ctrl->bg)
    free(ctrl->bg);
  ctrl->bg = tk_create_background(tk, full, left, right);
}

tk_control_t *tk_control_add(tk_screen_t *screen, char type, void *event_func,
                             const char *title) {
  tk_control_t *ctrl = malloc(sizeof(tk_control_t));

  ctrl->next = NULL;
  ctrl->title = title;
  ctrl->type = type;
  ctrl->data = NULL; // depends on type, TODO
  ctrl->bg = NULL;
  ctrl->event_func = event_func; // TODO

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
  if (ctrl->data)
    free(ctrl->data);
  free(ctrl);
  return todo;
}

void tk_control_next(tk_screen_t *screen) {
  screen->selected_control = screen->selected_control->next
                                 ? screen->selected_control->next
                                 : screen->first_control;
}

void tk_control_prev(tk_screen_t *screen) {
  tk_control_t *selected = screen->selected_control;
  tk_control_t *listpos = screen->first_control;

  // first entry -> get the last one!
  if (selected == listpos)
    while (listpos->next)
      listpos = listpos->next;
  else
    while (listpos && listpos->next != selected)
      listpos = listpos->next;

  screen->selected_control = listpos;
}
