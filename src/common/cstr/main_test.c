#include "cstr.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  char *merged =
      cstr_merge("hello", " world!\n", "look", "a", "merged", "string");

  printf("%s\n", merged);

  free(merged);
}
