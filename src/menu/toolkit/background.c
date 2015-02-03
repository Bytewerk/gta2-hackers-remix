#include "toolkit.h"
#include <SDL2/SDL_image.h>

// this can simply be deallocate this with free(),
// tk_cleanup() takes care of the textures
tk_background_t *tk_create_background(tk_t *tk, const char *full,
                                      const char *left, const char *right) {
  tk_background_t *bg = malloc(sizeof(tk_background_t));
  bg->full = full ? tk_texture_get(tk, full) : NULL;
  bg->left = left ? tk_texture_get(tk, left) : NULL;
  bg->right = right ? tk_texture_get(tk, right) : NULL;
  return bg;
}

// Cache GTA2 backgrounds as textures
void tk_init_gta2_background(tk_t *tk, const char *name) {
  tk_texture_t *tex = malloc(sizeof(tk_texture_t));
  tex->name = name;
  tex->next = NULL;

  char fullpath[100];
  snprintf(fullpath, sizeof(fullpath), "data/frontend/%s.tga", name);
  printf("loading %s...\n", fullpath);

  SDL_Surface *surface = IMG_Load(fullpath);
  if (!surface)
    exit(printf("File read error!\n", fullpath));

  // Workaround to fix reported "wrong colors" bug,
  // until it gets fixed upstream:
  //		https://bugzilla.libsdl.org/show_bug.cgi?id=2840
  // Pixels in the TGA are two bytes wide, and to
  // correct the colors, they need to be switched!

  char *pixels = surface->pixels;
  for (int y = 0; y < surface->h; y++)
    for (int x = 0; x < surface->w; x++) {
      int left_byte_addr = y * surface->w * 2 + x * 2;
      char left_old = pixels[left_byte_addr];

      pixels[left_byte_addr + 0] = pixels[left_byte_addr + 1];
      pixels[left_byte_addr + 1] = left_old;
    }

  // Create a texture from the surface and free it afterwards
  tex->texture = SDL_CreateTextureFromSurface(tk->renderer, surface);
  tex->width = surface->w;
  tex->height = surface->h;
  SDL_FreeSurface(surface);

  // append the new texture to the list
  if (tk->textures) {
    tk_texture_t *listpos = tk->textures;
    while (listpos->next)
      listpos = listpos->next;
    listpos->next = tex;
  } else
    tk->textures = tex;
}
