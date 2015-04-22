#include "gxt.h"
#include <stdio.h>
#include <stdlib.h>

gxt_t *gxt_load(const char *filename) {
  gxt_t *gxt = malloc(sizeof(gxt_t));
  printf("STUB: loading %s\n", filename);

  return gxt;
}

void gxt_cleanup(gxt_t *gxt) {
  printf("STUB: cleanup\n");
  free(gxt);
}
