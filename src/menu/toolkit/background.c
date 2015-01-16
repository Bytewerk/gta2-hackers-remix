#include "toolkit.h"
#include <SDL2/SDL_image.h>

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
    exit(printf("Failed to load '%s'!\n", fullpath));

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
