#include "mmp.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test mmp_folder\n");
  mmp_t *mmp = mmp_init(argv[1]);

  int i = 0;
  mmp_t *current = mmp;
  while (current) {
    printf("%2i: %s (%s)\n", i, mmp_read(current, "MapFiles/Description"),
           mmp->source);
    i++;
    current = current->next;
  }

  mmp_cleanup(mmp);
}
