#include "sfx.h"

int main() {
  sfx_t *sfx = sfx_init();

  sfx_play(sfx, SFX_FSTYLE_CHEAT);

  sfx_cleanup(sfx);
}
