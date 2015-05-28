#include "cstr.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  printf("MERGE:\n");
  char *merged = cstr_merge("hello", " world!             ", "look", "a",
                            "merged", "string! :)");

  printf("\t%s\n", merged);

  printf("SPLIT (by space character):\n");
  cstr_split_t *split = cstr_split(merged, ' ', 0);
  for (int i = 0; i < split->count; i++)
    printf("\tpiece %i: %s\n", i, split->pieces[i]);

  cstr_split_free(split);
  free(merged);
}
