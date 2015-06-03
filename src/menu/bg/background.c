#include "background.h"
#include "../../common/fs/fs.h"
#include <SDL2/SDL_image.h>
#include <string.h>

void bg_load_single(char *path, char *name, char *ext, void *userdata) {
  bg_t *bg = malloc(sizeof(bg_t));
  bg->name = fs_get_filename_without_ext(name, ext);
  bg->next = NULL;

  // actually load the file
  SDL_Surface *surface = IMG_Load(path);
  if (!surface)
    exit(printf("ERROR: couldn't read image '%s'!\n", path));

  // Workaround for upstream TGA bug:
  // https://bugzilla.libsdl.org/show_bug.cgi?id=2840
  if (!strcmp(ext, "tga")) {
    char *pixels = surface->pixels;
    for (int y = 0; y < surface->h; y++)
      for (int x = 0; x < surface->w; x++) {
        int left_byte_addr = y * surface->w * 2 + x * 2;
        char left_old = pixels[left_byte_addr];

        pixels[left_byte_addr + 0] = pixels[left_byte_addr + 1];
        pixels[left_byte_addr + 1] = left_old;
      }
  }
  bg->surface = surface;

  // attach it to the list
  bg_init_t *list = (bg_init_t *)userdata;
  if (list->first)
    list->last->next = bg;
  else
    list->first = bg;
  list->last = bg;
}

bg_t *bg_init() {
  bg_init_t *userdata = calloc(1, sizeof(bg_init_t));

  fs_iterate_over_files_in_folder("GTA2/data/frontend", "tga", bg_load_single,
                                  (void *)userdata, 0);

  fs_iterate_over_files_in_folder("data/frontend", "png", bg_load_single,
                                  (void *)userdata, 0);

  bg_t *first = userdata->first;
  free(userdata);

  return first;
}

bg_t *bg_cached(bg_t *bg, const char *name) {
  if (!name)
    return NULL;

  while (strcmp(bg->name, name)) {
    bg = bg->next;
    if (bg)
      continue;

    printf("ERROR: background %s not cached!\n", name);
    return NULL;
  }
  return bg;
}

bg_mashup_t *bg_mashup(bg_t *bg, const char *full, const char *left,
                       const char *right, const char *custom) {
  bg_mashup_t *ret = malloc(sizeof(bg_mashup_t));
  ret->full = bg_cached(bg, full);
  ret->left = bg_cached(bg, left);
  ret->right = bg_cached(bg, right);
  ret->custom = bg_cached(bg, custom);
  return ret;
}

void bg_cleanup(bg_t *bg) {
  bg_t *next = bg;
  while (next) {
    bg = next;
    next = bg->next;
    SDL_FreeSurface(bg->surface);
    free(bg);
  }
}
