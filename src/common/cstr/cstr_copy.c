#include "cstr.h"
#include <stdlib.h>
#include <string.h>

char *cstr_copy(char *cstr) {
  if (cstr == NULL)
    return NULL;

  size_t len = strlen(cstr);
  char *ret = malloc(len + 1);
  memcpy(ret, cstr, len + 1);
  return ret;
}
