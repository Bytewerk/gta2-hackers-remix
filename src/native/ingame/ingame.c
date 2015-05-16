#include "ingame.h"
#include "../../common/api_native2injected.h"

ingame_t *ingame_init(net_t *net, inmenu_t *inmenu) {
  ingame_t *ingame = calloc(1, sizeof(ingame_t));
  ingame->net = net;
  ingame->inmenu = inmenu;

  return ingame;
}

int ingame_player_id_by_pid(uint16_t pid, uint16_t *table) {
  for (int i = 0; i < 6; i++)
    if (pid == table[i])
      return 1;
  return -1;
}

// make use of:
// ud->ingame
// instance->sock
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
      ud_instance->player_id =
          ingame_player_id_by_pid(data->pid, ingame->inmenu->pids);
      printf("[native] got pid: %i => must be player %i\n",
             ud_instance->instance_pid, ud_instance->player_id);
    });
  }
}

void ingame_frame(ingame_t *ingame) {
  // TODO: iterate over all instances, send movement bytes
}

void ingame_cleanup(ingame_t *ingame) { free(ingame); }
