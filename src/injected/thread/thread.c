#include "../mem/mem.h"
#include "../net/injected_net.h"
#include <windows.h>

void injected_thread(void *param) {
  // 0 on failure
  if (!net_init())
    return;

  // send the PID for identification
  SEND2NATIVE(IA_PID, data->pid = GetCurrentProcessId());

  mem_t *mem = mem_init();

  // net_recv_blocking is blocking until new data arrives, but the
  // native component sends the controller input at a constant rate
  while (net_recv_blocking(mem_recv_callback, (void *)mem)) {
    mem_frame(mem);
  }

  mem_cleanup(mem);
}
