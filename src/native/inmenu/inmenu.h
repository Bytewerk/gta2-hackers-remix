#pragma once
#include "../net/native_net.h"
#include "../pads/pads.h"

typedef struct {
  net_t *net;
  pads_t *pads;
  uint16_t pids[6];
  char has_quit;

  bool singleplayer;
} inmenu_t;

inmenu_t *inmenu_init(net_t *net, pads_t *pads);

void inmenu_recv_callback(unsigned char msg_id, TCPsocket sock, void *userdata);

void inmenu_frame(inmenu_t *inmenu, SDL_Event *event);
void inmenu_frame_timeout(inmenu_t *inmenu);

void inmenu_cleanup(inmenu_t *inmenu);
