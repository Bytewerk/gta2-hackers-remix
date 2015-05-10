#pragma once

// prefpath must have a trailing slash. use SDL_GetPrefPath() for that
// (and don't forget to free the string!)
void rpc_init(char *prefpath);
