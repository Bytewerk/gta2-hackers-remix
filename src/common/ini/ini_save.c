#include "../cstr/cstr.h"
#include "ini.h"
#include <stdio.h>

#define WRITE_LINE(...)                                                        \
  {                                                                            \
    char *line = cstr_merge(__VA_ARGS__);                                      \
    fputs(line, handle);                                                       \
    free(line);                                                                \
  }

void ini_save(ini_t *ini, char *fullpath, bool noprotip, bool quiet) {
  if (!fullpath)
    fullpath = ini->fullpath;
  if (!fullpath)
    printf("ERROR: Can't save the ini-file, because it"
           " has no fullpath inside the struct, and no fullpath was passed"
           " to ini_save()!\n");
  if (!quiet)
    printf("writing %s...\n", fullpath);

  FILE *handle = fopen(fullpath, "w");
  if (!noprotip)
    fputs("; PROTIP: Don't modify this file, it gets"
          " written automagically!\n\n",
          handle);

  ini_section_t *section = ini->sections;
  while (section) {
    WRITE_LINE("[", section->name, "]\n");

    ini_entry_t *entry = section->entries;
    while (entry) {
      WRITE_LINE(entry->key, " = ", entry->value, "\n");

      entry = entry->next;
    }

    fputc('\n', handle);
    section = section->next;
  }

  fclose(handle);
}

#undef WRITE_LINE
