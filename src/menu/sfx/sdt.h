#include <SDL2/SDL_mixer.h>

typedef struct {
  Mix_Chunk **chunks;
  int count;
} sfx_sdt_t;

// path: must have a trailing slash
// name: the filename, without the extensions (we load 2 files!)
sfx_sdt_t *sfx_sdt_load(const char *path, const char *name);

void sfx_sdt_cleanup(sfx_sdt_t *sdt);
