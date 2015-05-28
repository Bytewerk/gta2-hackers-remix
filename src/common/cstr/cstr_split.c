#include "cstr.h"
#include <stdlib.h>
#include <string.h>

cstr_split_t *cstr_split(char *cstr, char delimeter, bool trim /*todo*/) {
  size_t len = strlen(cstr);

  // count all occurences
  uint16_t count = 0;
  for (size_t i = 1; i < len; i++) {
    if (cstr[i] == delimeter && cstr[i - 1] != delimeter)
      count++;
  }

  // actually cut the string
  char **pieces = malloc(sizeof(char *) * (count + 1));
  size_t pos_cut = 0;
  uint16_t filled = 0;
  for (size_t i = 1; i <= len; i++) {
    if (cstr[i - 1] == delimeter)
      pos_cut++;
    else if (cstr[i] == delimeter || i == len) {
      size_t len_substr = i - pos_cut;
      pieces[filled] = malloc(len_substr + 1);

      memcpy(pieces[filled], cstr + pos_cut, len_substr);
      pieces[filled][len_substr] = '\0';

      // TODO: trim

      filled++;
      pos_cut = i;
    }
  }

  // return as cstr_split_t struct
  cstr_split_t *split = malloc(sizeof(cstr_split_t));
  split->count = count + 1;
  split->pieces = pieces;
  return split;
}

void cstr_split_free(cstr_split_t *split) {
  for (uint16_t i = 0; i < split->count; i++)
    free(split->pieces[i]);
  free(split->pieces);
  free(split);
}
