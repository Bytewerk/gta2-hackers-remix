#pragma once
#include "../cmap/cmap.h"
#include "../net/native_net.h"
#include "../pads/pads.h"

void ieh_frame(net_t *net, pads_t *pads, cmap_t *cmap, SDL_Event *event);
