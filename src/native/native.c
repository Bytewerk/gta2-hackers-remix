#include "net/net.h"
#include <stdio.h>

int main(int argc, char **argv) {
  net_t *net = net_init();

  printf("cleaning up...\n");
  net_cleanup(net);

  printf("cleaned up!\n");
}
