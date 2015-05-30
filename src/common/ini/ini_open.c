#include "../cstr/cstr.h"
#include "../io/io.h"
#include "ini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void ini_parse(ini_t *ini, char *buffer, uint32_t size) {
  bool ignore_line;
  uint32_t bracket_left = -1;
  uint32_t equals_pos = -1;
  uint32_t line_start = 0;
  ini_section_t *section = NULL;
  ini_entry_t *entry = NULL;

  for (uint32_t i = 0; i <= size; i++) {
    // pretend that there's a new line at the end
    char c = i < size ? buffer[i] : '\n';

    if (c == '[')
      bracket_left = i + 1;
    else if (c == ';' && i == line_start)
      ignore_line = true;
    else if (c == ']') {
      // new section
      if (bracket_left == -1)
        exit(printf("ERROR: unexpected ']'"
                    "without '[' in the same line!"));
      ini_section_t *new = calloc(1, sizeof(ini_section_t));
      uint16_t len = i - bracket_left;
      new->name = malloc(len + 1);
      memcpy(new->name, buffer + bracket_left, len);
      new->name[len] = '\0';

      // attach it to the list
      if (section)
        section->next = new;
      else
        ini->sections = new;
      section = new;
      entry = NULL;
    } else if (c == '\n' || c == '\r') {
      if (equals_pos != -1 && !ignore_line) {
        // add the new entry to the list
        ini_entry_t *new = calloc(1, sizeof(ini_entry_t));
        if (entry)
          entry->next = new;
        else
          section->entries = new;
        entry = new;

        // set the key
        uint32_t key_len = equals_pos - line_start;
        new->key = malloc(key_len + 1);
        memcpy(new->key, buffer + line_start, key_len);
        new->key[key_len] = '\0';
        cstr_trim(new->key);

        // set the value
        uint32_t value_len = i - equals_pos - 1;
        new->value = malloc(value_len + 1);
        memcpy(new->value, buffer + equals_pos + 1, value_len);
        new->value[value_len] = '\0';
        cstr_trim(new->value);
      }
      ignore_line = false;
      bracket_left = -1;
      equals_pos = -1;
      line_start = i + 1;
    } else if (c == '=') {
      equals_pos = i;
    }
  }
}

ini_t *ini_open(char *fullpath, bool copy_fullpath, bool quiet) {
  // immediatelly return if the file doesn't exist
  ini_t *ini = calloc(1, sizeof(ini_t));
  ini->fullpath = copy_fullpath ? cstr_copy(fullpath) : fullpath;
  if (!fullpath || access(fullpath, 0))
    return ini;

  uint32_t size;
  char *buffer = io_load_small_file_to_ram(fullpath, &size, quiet);

  ini_parse(ini, buffer, size);

  free(buffer);
  return ini;
}
