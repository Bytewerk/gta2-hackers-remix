#include "cstr.h"
#include <ctype.h>
#include <string.h>

char *cstr_trim(char *cstr) {
  // find 'isspace'-chars at the beginning and end
  size_t len = strlen(cstr);
  size_t trim_start = 0;
  size_t trim_end = len;
  bool non_space_found = false;
  for (size_t i = 0; i < len; i++) {
    if (isspace(cstr[i])) {
      if (!non_space_found && (!trim_start || trim_start == i))
        trim_start = i + 1;
      else if (non_space_found && trim_end == len)
        trim_end = i;
    } else {
      non_space_found = true;
      trim_end = len;
    }
  }

  // move all chars to the left, cut the string off
  for (size_t i = 0; i < len; i++) {
    if (i + trim_start == trim_end) {
      cstr[i] = '\0';
      break;
    }

    // move chars to the left
    cstr[i] = cstr[i + trim_start];
  }

  // just keep the additional bytes, they don't hurt.
  return cstr;

  // less memory / more cpu alternative:
  // return realloc(cstr, trim_end - trim_start + 1);
}
