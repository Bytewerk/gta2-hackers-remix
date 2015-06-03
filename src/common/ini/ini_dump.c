#include "ini.h"
#include <stdio.h>

void ini_dump(ini_t *ini) {
  printf("debug ini dump:\n");
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
}
