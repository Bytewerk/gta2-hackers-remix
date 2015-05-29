#include "ini.h"
#include <stdlib.h>

void ini_cleanup(ini_t *ini) {
  ini_section_t *section = ini->sections;
  while (section) {
    ini_section_t *old = section;
    section = old->next;

    ini_entry_t *entry = old->entries;
    while (entry) {
      ini_entry_t *old = entry;
      entry = old->next;
      free(old->key);
      free(old->value);
      free(old);
    }
    free(old->name);
    free(old);
  }
  free(ini);
}
