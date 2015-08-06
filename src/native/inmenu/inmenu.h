#pragma once
#include "../../common/headers/tk_actions.h"
#include "../net/native_net.h"
#include "../pads/pads.h"

typedef struct {
  net_t *net;
  pads_t *pads;
  uint16_t pids[6];
  char has_quit;
  uint32_t analogstick_timer[TK_ACTION_LAST - 1];

  bool singleplayer;
} inmenu_t;

inmenu_t *inmenu_init(net_t *net, pads_t *pads);

void inmenu_recv_callback(unsigned char msg_id, TCPsocket sock, void *userdata);

void inmenu_frame(inmenu_t *inmenu, SDL_Event *event);
void inmenu_frame_timeout(inmenu_t *inmenu);

void inmenu_cleanup(inmenu_t *inmenu);
