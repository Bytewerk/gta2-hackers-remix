#include "ingame.h"
#include "../../common/api_native2injected.h"

ingame_t *ingame_init(net_t *net) {
  ingame_t *ingame = calloc(1, sizeof(ingame_t));
  ingame->net = net;

  return ingame;
}

// make use of:
// ud->ingame
// instance->sock
void ingame_recv_callback(unsigned char msg_id,
                          net_injected_instance_t *instance) {
  printf("[native] ingame recv callback stub. msg_id: %i\n", msg_id);

  // ingame_instance_userdata_t* ud =
  //	(ingame_instance_userdata_t*) instance->userdata;

  // TODO: create userdata, if it is empty!
  // TODO: actually handle the message, we'll need a
  // FRAMEDATACASE like macro here.

  /*

  switch(msg_id)
  {
          FRAMEDATACASE(IA_PID,
          {
                  session->instance_pid = data->pid;
                  printf("got pid: %i\n", data->pid);

                  // TODO: try to look up player number by pid now
          });

  }

  */
}

void ingame_frame(ingame_t *ingame) {
  // TODO: iterate over all instances, send movement bytes
}

void ingame_cleanup(ingame_t *ingame) { free(ingame); }
