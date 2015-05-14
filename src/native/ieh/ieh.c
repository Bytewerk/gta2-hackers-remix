#include "ieh.h"
#include "../../common/api_native2injected.h"

void ieh_frame(net_t *net, pads_t *pads, cmap_t *cmap, SDL_Event *event) {
  // only do something, when the event timeout was reached,
  // so we have a constant rate of input!
  if (event->type)
    return;

  // what we need here:
  // - player count
  // - is player driving?

  // TODO: iterate over all players, check button and axis state,
  // generate movement actions
}
