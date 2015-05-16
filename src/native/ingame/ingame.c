#include "ingame.h"
#include "../../common/api_native2injected.h"
#include "../../common/common.h"

ingame_t *ingame_init(net_t *net, pads_t *pads, inmenu_t *inmenu) {
  ingame_t *ingame = calloc(1, sizeof(ingame_t));
  ingame->net = net;
  ingame->pads = pads;
  ingame->inmenu = inmenu;

  return ingame;
}

int ingame_player_id_by_pid(uint16_t pid, uint16_t *table) {
  for (int i = 0; i < 6; i++)
    if (pid == table[i])
      return i;
  return -1;
}

void ingame_recv_callback(unsigned char msg_id,
                          net_injected_instance_t *instance, void *userdata) {
  TCPsocket sock = instance->sock;
  ingame_instance_userdata_t *ud_instance =
      (ingame_instance_userdata_t *)instance->userdata;
  ingame_t *ingame = (ingame_t *)userdata;

  if (!ud_instance) {
    ud_instance = malloc(sizeof(ingame_instance_userdata_t));
    ud_instance->player_id = -1;
    instance->userdata = (void *)ud_instance;
  }

  switch (msg_id) {
    MESSAGECASE(sock, IA_PID, {
      ud_instance->instance_pid = data->pid;

      int player_id = ingame_player_id_by_pid(data->pid, ingame->inmenu->pids);

      if (player_id > -1 && player_id < GTA2_PLAYER_COUNT) {
        ud_instance->player_id = player_id;
        printf("[native] got pid: %i => must be player %i\n", data->pid,
               player_id);

        MESSAGESEND(sock, IA_PLAYER_ID, data->id = ud_instance->player_id);

        ingame->instance_by_player_id[player_id] = instance;

        // store the array location, so it can get NULL'd on
        // disconnect by net_frame().
        instance->sorted_array_location =
            &(ingame->instance_by_player_id[player_id]);
      } else
        printf("[native] ERROR: got pid %i, but can't find"
               " the associated player ID!\n",
               data->pid);
    });
  }
}

void ingame_frame(ingame_t *ingame, SDL_Event *event) {
  // only send the current movement when the event timeout has
  // been reached!
  if (event)
    return;

  int count = ingame->net->injected_count;

  for (int i = 0; i < count; i++) {
    net_injected_instance_t *instance = ingame->instance_by_player_id[i];
    if (!instance)
      continue;

    // FIXME: NOW SEND MOVEMENT DATA
  }
}

void ingame_cleanup(ingame_t *ingame) { free(ingame); }
