#pragma once
#include <stdio.h>

// prefpath must have a trailing slash. use SDL_GetPrefPath() for that
// (and don't forget to free the string!)
void rpc_init(char *prefpath);

typedef struct rpc_pos_t {
  struct rpc_pos_t *next;
  size_t pos;
} rpc_pos_t;

#define G2HR_VIKE_PATCH "data/vike_patch/vike_patch.plainbsdiff"
