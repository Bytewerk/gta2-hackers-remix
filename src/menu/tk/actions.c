#include "actions.h"
#include "toolkit.h"
#include <SDL2/SDL.h>

/*
        A lot of SDL events get converted to toolkit actions here.
        For example the "back" action can be caused by keyboard (esc, backslash)
   and gamepad (over tcp).
*/
tk_action_t convert(tk_t *tk, SDL_Event *e) {
  if (e->type == SDL_WINDOWEVENT)
    return TK_ACTION_REDRAW;

  if (e->type == SDL_MOUSEMOTION)
    return TK_ACTION_MOUSEMOVE;

  if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
    return TK_ACTION_MOUSEDOWN;

  if (e->type == SDL_KEYDOWN) {
    SDL_Keycode key = e->key.keysym.sym;
    if (key == SDLK_UP)
      return TK_ACTION_UP;
    if (key == SDLK_DOWN)
      return TK_ACTION_DOWN;
    if (key == SDLK_LEFT)
      return TK_ACTION_LEFT;
    if (key == SDLK_RIGHT)
      return TK_ACTION_RIGHT;
    if (key == SDLK_ESCAPE || key == SDLK_BACKSPACE)
      return TK_ACTION_BACK;
    if (key == SDLK_RETURN)
      return TK_ACTION_ENTER;
  }

  return TK_ACTION_NONE;
}

void actions_element(tk_t *tk, tk_el_t *el, tk_action_t action) {
  void (*actionfunc)(tk_t *, tk_el_t *, tk_action_t) = el->actionfunc;
  if (!actionfunc)
    return;
  actionfunc(tk, el, action);
}

void actions_recursive(tk_t *tk, tk_el_t *el, tk_action_t action) {
  if (!el)
    return;

  // STUB
}

void tk_action(tk_t *tk, SDL_Event *event) {
  tk_action_t action = convert(tk, event);
  printf("action id: %i\n", action);

  // single action for the active screen
  actions_element(tk, &(tk->screen_active->el), action);

  // action for all selected elements, recursively
  actions_recursive(tk, tk->screen_active->el_selected, action);
}
