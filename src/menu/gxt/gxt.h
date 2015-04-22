#pragma once
#include <stdint.h>

typedef struct {

} gxt_t;

gxt_t *gxt_load(const char *filename);
void gxt_cleanup(gxt_t *gxt);
