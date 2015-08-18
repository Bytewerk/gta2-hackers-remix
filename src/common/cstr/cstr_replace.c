#include "cstr.h"
#include <stdlib.h>
#include <string.h>

char *cstr_replace(char *cstr, char find, char replace, uint16_t count,
                   bool copy) {
  char *ret = copy ? cstr_copy(cstr) : cstr;

  char *pos = ret;
  uint16_t replaced = 0;
  while (1) {
    if (*pos == find) {
      *pos = replace;
      replaced++;
      if (count && count <= replaced)
        break;
    }
    if (!*pos)
      break;
    pos++;
  }
  return ret;
}
