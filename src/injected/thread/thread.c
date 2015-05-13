#include "../gta2_memory.h"
#include "../net/injected_net.h"

void injected_thread(void *param) {
  net_init();

  // TODO: send the PID for identification

  // TODO: injected ingame loop
}
