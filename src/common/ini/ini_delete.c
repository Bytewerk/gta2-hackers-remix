bool ini_delete_key(ini_t *ini, char *section_name, char *key) {
  ini_section_t *section = ini->sections;
  while (section) {
    if (!strcmp(section->name, section_name)) {
      ini_entry_t *prev = NULL;
      ini_entry_t *entry = section->entries;
      while (entry) {
        if (!strcmp(entry->key, key)) {
          if (prev)
            prev->next = entry->next;
          else
            section->entries = NULL;
          free(entry->key);
          free(entry->value);
          free(entry);
          return true;
        }
        prev = entry;
        entry = entry->next;
      }
      return false;
    }
    section = section->next;
  }
  return false;
}
