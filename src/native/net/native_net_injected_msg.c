#include "../../common/headers/api_native2injected.h"
#include "native_net.h"

// This runs in an extra thread. SDL2_net isn't threadsafe, so just
// set a flag here that the message should be hidden. The main thread
// calls the clear function then.
uint32_t net_injected_msg_clear_callback(uint32_t interval, void *userdata) {
  ((net_injected_instance_t *)userdata)->msg_clear_flag = true;
  return 0;
}

void net_injected_msg_clear_timer(net_injected_instance_t *instance) {
  SDL_RemoveTimer(instance->msg_clear_timer);
  instance->msg_clear_flag = false;
  instance->msg_clear_timer =
      SDL_AddTimer(1000, net_injected_msg_clear_callback, instance);
}

void net_injected_msg_set(net_injected_instance_t *instance, bool autoclear,
                          char *line1_max11, char *line2_max33,
                          char *line3_max33) {
  MESSAGESEND(instance->sock, IA_ESC_TEXT_SHOW,
              strncpy(data->line1, line1_max11 ? line1_max11 : "", 11);
              strncpy(data->line2, line2_max33 ? line2_max33 : "", 33);
              strncpy(data->line3, line3_max33 ? line3_max33 : "", 33););

  if (autoclear)
    net_injected_msg_clear_timer(instance);
}

void net_injected_msg_clear(net_injected_instance_t *instance) {
  SDL_RemoveTimer(instance->msg_clear_timer);
  instance->msg_clear_flag = false;

  // this only works right when sent twice?
  for (int i = 0; i < 2; i++) {
    MESSAGESEND(instance->sock, IA_ESC_TEXT_SHOW, strncpy(data->line1, "", 11);
                strncpy(data->line2, "", 33); strncpy(data->line3, "", 33););
  }
}
