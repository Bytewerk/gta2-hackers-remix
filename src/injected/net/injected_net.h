#pragma once

// returns 0 on success, a higher number on failure
int net_init();

// gets called, when the DLL gets unloaded
void net_cleanup();
