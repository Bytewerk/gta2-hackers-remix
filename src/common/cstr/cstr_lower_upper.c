#include "cstr.h"
#include <ctype.h>
#include <stdlib.h>

char *cstr_lower_upper(char *cstr, bool copy, bool lower) {
  char *ret = copy ? cstr_copy(cstr) : cstr;
  char *pos = ret;

  while (*pos) {
    *pos = lower ? tolower(*pos) : toupper(*pos);
    pos++;
  }

  return ret;
}
