#include "ingame.h"
#include "../../common/api_native2injected.h"

ingame_t *ingame_init(net_t *net, inmenu_t *inmenu) {
  ingame_t *ingame = calloc(1, sizeof(ingame_t));
  ingame->net = net;
  ingame->inmenu = inmenu;

  return ingame;
}

// make use of:
// ud->ingame
// instance->sock
void ingame_recv_callback(unsigned char msg_id,
                          net_injected_instance_t *instance) {
  TCPsocket sock = instance->sock;
  ingame_instance_userdata_t *ud =
      (ingame_instance_userdata_t *)instance->userdata;

  if (!ud) {
    ud = malloc(sizeof(ingame_instance_userdata_t));
    ud->player_id = -1;
    instance->userdata = (void *)ud;
  }

  switch (msg_id) {
    MESSAGECASE(sock, IA_PID, {
      ud->instance_pid = data->pid;
      printf("[native] got pid: %i\n", data->pid);
      // TODO: look up the player id!
    });
  }
}

void ingame_frame(ingame_t *ingame) {
  // TODO: iterate over all instances, send movement bytes
}

void ingame_cleanup(ingame_t *ingame) { free(ingame); }
