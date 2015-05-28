#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// the variable arguments list can only be iterated through once, so
// simply save all strings in a list - and add the string length while
// we're at it, so this only needs to be done once, too.
typedef struct cstr_merge_internal_t {
  struct cstr_merge_internal_t *next;
  char *cstr;
  size_t len;
} cstr_merge_internal_t;

char *cstr_merge_internal(char *arg_start, ...) {
  cstr_merge_internal_t *first = NULL;
  cstr_merge_internal_t *last = NULL;
  va_list args;
  size_t len = 0;

  // fill the list, calculate the total size
  va_start(args, arg_start);
  while (1) {
    char *current = va_arg(args, char *);
    if (!current)
      break;
    size_t current_len = strlen(current);
    len += current_len;

    // new list entry
    cstr_merge_internal_t *entry = malloc(sizeof(cstr_merge_internal_t));
    entry->next = NULL;
    entry->cstr = current;
    entry->len = current_len;

    // add it to the list
    if (first)
      last->next = entry;
    else
      first = entry;
    last = entry;
  }
  va_end(args);

  // create the new string and free the list
  char *ret = malloc(len + 1);
  size_t written = 0;
  while (first) {
    cstr_merge_internal_t *current = first;
    memcpy(ret + written, current->cstr, current->len);
    written += current->len;

    first = current->next;
    free(current);
  }
  ret[written] = '\0';

  return ret;
}
