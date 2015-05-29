#include "ini.h"
#include <string.h>

char *ini_read(ini_t *ini, char *section_name, char *key) {
  ini_section_t *section = ini->sections;
  while (section) {
    if (!strcmp(section->name, section_name)) {
      ini_entry_t *entry = section->entries;
      while (entry) {
        if (!strcmp(entry->key, key))
          return entry->value;
        entry = entry->next;
      }
    }
    section = section->next;
  }

  return "";
}
