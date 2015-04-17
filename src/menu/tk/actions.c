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
    if (key == SDLK_ESCAPE)
      return TK_ACTION_ESC;
    if (key == SDLK_BACKSPACE)
      return TK_ACTION_BACKSPACE;
    if (key == SDLK_RETURN)
      return TK_ACTION_ENTER;

    // http://wiki.libsdl.org/SDLKeycodeLookup
    if (key >= SDLK_SPACE && key <= SDLK_z)
      return TK_ACTION_TYPING;
  }

  return TK_ACTION_NONE;
}

void actions_element(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                     tk_action_t action, SDL_Keycode key) {
  void (*actionfunc)(tk_t *, tk_el_t *, tk_el_t *, tk_action_t,
                     SDL_Keycode key) = el->actionfunc;
  if (actionfunc)
    actionfunc(tk, el, el_selected, action, key);
}

void actions_recursive(tk_t *tk, tk_el_t *el, tk_el_t *el_selected,
                       tk_action_t action, SDL_Keycode key) {
  while (el) {
    if (el->sub)
      actions_recursive(tk, el->sub, el_selected, action, key);
    actions_element(tk, el, el_selected, action, key);
    el = el->next;
  }
}

void tk_action(tk_t *tk, SDL_Event *event) {
  tk_action_t action = convert(tk, event);

  // action limit rate, when necessary
  if (action < TK_ACTION_LAST_LIMITED) {
    uint32_t current_time = SDL_GetTicks();
    if (current_time - tk->action_time[action] < ACTION_LIMIT_RATE)
      return;
    tk->action_time[action] = current_time;
  }

  tk_el_t *el_selected = tk->screen_active->el_selected;
  SDL_Keycode key = event->key.keysym.sym;

  if (tk->exclusive_action_element) {
    actions_element(tk, tk->exclusive_action_element, el_selected, action, key);
    return;
  }

  // single action for the active screen
  actions_element(tk, &(tk->screen_active->el), el_selected, action, key);

  // action for all selected elements, recursively
  actions_recursive(tk, tk->screen_active->el_selected, el_selected, action,
                    key);
}
