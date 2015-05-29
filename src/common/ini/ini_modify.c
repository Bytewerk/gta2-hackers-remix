#include "ini.h"

void ini_modify(ini_t *ini, char *section_name, char *key, char *value,
                bool overwrite) {
  ini_entry_t *entry = NULL;

  // find the section
  ini_section_t *section = ini->sections;
  while (section) {
    if (strcmp(section->name, section_name))
      section = section->next;
    else
      break;
  }

  // ...or add a new one
  if (!section) {
    ini_section_t *section = calloc(1, sizeof(ini_section_t));
    section->next = ini->sections;
    section->name = section_name;
    ini->sections = section;
  } else // find the entry
  {
    while (entry) {
      if (strcmp(entry->key, key))
        entry = entry->next;
      else
        break;
    }

    if (entry && !overwrite)
      return;
  }

  // add a new entry if necessary
  if (!entry) {
    entry = calloc(1, sizeof(ini_entry_t));
    entry->next = section->entries;
    entry->key = key;
    section->entries = entry;
  }
  entry->value = value;
}
