#include "../net/injected_net.h"
#include <windows.h>

void injected_thread(void *param) {
  // >0 on failure
  if (net_init())
    return;

  // send the PID for identification
  SEND2NATIVE(IA_PID, data->pid = GetCurrentProcessId());

  // TODO: injected ingame loop
}
