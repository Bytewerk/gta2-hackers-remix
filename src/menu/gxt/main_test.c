#include "gxt.h"
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test gta2_text_file\n");
  gxt_t *gxt = gxt_load(argv[1]);

  /*
  gxt_entry_t* entry = gxt->entries;
  int i=0;
  while(entry)
  {
          printf("(%5i) %10s: %s\n", i, entry->id, entry->text);


          entry = entry->next;
          i++;
  }
  */

  printf("finding gxt str...\n");
  char *str = gxt_get(gxt, "play");

  printf("printing gxt str...\n");
  printf("%s\n", str);

  gxt_cleanup(gxt);
  printf("cleaned up!\n");
}
