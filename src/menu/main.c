#include "menu.h"
#include "sty_parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  sty_t *sty = sty_parser("fstyle.sty");

  sty_cleanup(sty);
  printf("Cleaned up!\n");
  return 0;
}
