#include "ini.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test ini_file\n");

  ini_t *ini = ini_open(argv[1], 0);

  printf("full dump:\n");
  ini_section_t *section = ini->sections;
  while (section) {
    printf("\t'%s':\n", section->name);
    ini_entry_t *entry = section->entries;
    while (entry) {
      printf("\t\t'%s': '%s'\n", entry->key, entry->value);
      entry = entry->next;
    }
    section = section->next;
  }

  printf("only accessing slotmachine/enabled:\n");
  printf("\t'%s'\n", ini_read(ini, "slotmachine", "enabled"));

  printf("cleaning up...\n");
  ini_cleanup(ini);
  printf("done!\n");
}
