#pragma once
#include "../../common/headers/common.h"
#include "../cmap/cmap.h"
#include "../inmenu/inmenu.h"
#include "../net/native_net.h"
#include "../pads/pads.h"
#include <stdbool.h>

typedef struct {
  net_t *net;
  cmap_t *cmap;
  pads_t *pads;
  inmenu_t *inmenu;
  net_injected_instance_t *instance_by_player_id[GTA2_PLAYER_COUNT];

  // score table etc.
} ingame_t;

typedef struct {
  int player_id;    // -1 if unset
  int instance_pid; // 0 if unset
  bool driving;     // 0: walking, 1: driving
  char is_in_quit_dialog;

  // TODO: put selected game controller layout here
} ingame_instance_userdata_t;

ingame_t *ingame_init(net_t *net, cmap_t *cmap, pads_t *pads, inmenu_t *inmenu);

// on the first call of a new session, the instance userdata is NULL and
// will be allocated in this function. it gets free'd by the network
// functions on disconnect.
void ingame_recv_callback(unsigned char msg_id,
                          net_injected_instance_t *instance, void *userdata);

// TODO: add SDL_Event, like in inmenu.h?
void ingame_frame(ingame_t *ingame, SDL_Event *event);
void ingame_cleanup(ingame_t *ingame);
