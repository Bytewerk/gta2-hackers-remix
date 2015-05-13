#include "cfg.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char *cfg_read(cfg_t *cfg, char *key) {
  while (cfg) {
    if (!strcmp(cfg->key, key))
      return cfg->value;
    cfg = cfg->next;
  }
  return "";
}

// little helping function that removes whitespaces around a string,
// for example: ' hello world!  ' becomes 'hello world!'
char *cfg_trim(char *str, int from) {
  // find whitespace positions, that we'll rip off the string
  int len = strlen(str);
  int trimmed_start = 0;
  int trimmed_end = len;
  char non_space_found = 0;
  for (int i = from; i < len; i++) {
    if (isspace(str[i])) {
      if (!non_space_found && (!trimmed_start || trimmed_start == i))
        trimmed_start = i + 1;
      else if (non_space_found && trimmed_end == len)
        trimmed_end = i;
    } else {
      non_space_found = 1;
      trimmed_end = len;
    }
  }

  // now move the chars to the left and cut the string off
  int offset_start = trimmed_start - from;
  if (offset_start < 0)
    offset_start = 0;

  for (int i = from; i < len; i++) {
    if (i + offset_start == trimmed_end) {
      str[i] = '\0';
      break;
    }
    str[i] = str[i + offset_start];
  }

  // trading a few bytes for additional CPU time. For G2HR this won't
  // really matter anyway, except when you have a gazillion custom
  // maps in your data folder.
  return realloc(str, trimmed_end - offset_start + 1);
}

cfg_t *cfg_parse(char *buffer, int size) {
  cfg_t *first = NULL;
  cfg_t *last = NULL;

  char ignore_line = 0;

  int start_brackets = -1;
  int start_key = -1;
  int start_line = 0;

  char *section = NULL;
  int section_length = 0;
  for (int i = 0; i <= size; i++) {
    char c = i < size ? buffer[i] : '\n'; // add an extra line end
    if (c == '[')
      start_brackets = i + 1;
    else if (c == ';' && i == start_line)
      ignore_line = 1;
    else if (c == ']') {
      if (start_brackets == -1)
        exit(printf("ERROR: unexpected ']' without '[' in the same line!"));

      if (section)
        free(section);
      section_length = i - start_brackets;
      section = malloc(section_length + 1);
      strncpy(section, buffer + start_brackets, section_length);
      section[section_length] = '\0';
    } else if (c == '\n' || c == '\r') {
      if (start_key != -1 && !ignore_line) {
        int key_length = start_key - start_line - 1;
        int key_section_length = section_length + 1 /* slash */
                                 + key_length;
        int value_length = i - start_key;

        cfg_t *new = malloc(sizeof(cfg_t));
        new->next = NULL;
        new->key = malloc(key_section_length + 1);
        new->value = malloc(value_length + 1);

        // create 'key' string
        strncpy(new->key, section, section_length);
        strncpy(new->key + section_length + 1, buffer + start_line, key_length);
        new->key[section_length] = '/';
        new->key[key_section_length] = '\0';
        new->key = cfg_trim(new->key, section_length + 1);

        // create 'value' string
        strncpy(new->value, buffer + start_key, value_length);
        new->value[value_length] = '\0';
        new->value = cfg_trim(new->value, 0);

        // add it to the list
        if (last)
          last->next = new;
        else
          first = new;
        last = new;
      }
      ignore_line = 0;
      start_brackets = -1;
      start_key = -1;
      start_line = i + 1;
    } else if (c == '=')
      start_key = i + 1;
  }

  if (section)
    free(section);
  return first;
}

cfg_t *cfg_load(char *filename, char quiet) {
  if (!quiet)
    printf("loading %s...\n", filename);
  FILE *handle = fopen(filename, "rb");
  if (!handle)
    exit(printf("ERROR: Couldn't read '%s'!\n", filename));

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(size);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error while reading '%s'!\n", filename));
  fclose(handle);

  cfg_t *cfg = cfg_parse(buffer, size);
  free(buffer);
  return cfg;
}

void cfg_cleanup(cfg_t *cfg) {
  while (cfg) {
    cfg_t *old = cfg;
    cfg = cfg->next;
    free(old->key);
    free(old->value);
    free(old);
  }
}

// returns NULL if the key was not found
cfg_split_t *cfg_split_value(cfg_t *cfg, char *key, char delimeter) {
  char *value = cfg_read(cfg, key);
  if (!value[0]) {
    // FIXME: make this warning optional!
    printf("WARNING: Can't find value for %s in config!\n", key);
    return NULL;
  }

  // count the split values
  int length = strlen(value);
  int count = 0;
  for (int i = 1; i < length; i++)
    if (value[i] == delimeter && value[i - 1] != delimeter)
      count++;

  // actually cut the string
  char **split_values = malloc(sizeof(char *) * (count + 1));
  int cutoff_start = 0;
  int filled = 0;
  for (int i = 0; i <= length; i++) {
    if ((value[i] == delimeter && value[i - 1] != delimeter) || i == length) {
      int length_substr = i - cutoff_start;
      split_values[filled] = calloc(1, length_substr + 1);
      strncpy(split_values[filled], value + cutoff_start, length_substr);
      split_values[filled] = cfg_trim(split_values[filled], 0);
      filled++;
      cutoff_start = i + 1;
    }
  }

  cfg_split_t *split = malloc(sizeof(cfg_split_t));
  split->values = split_values;
  split->count = filled;
  return split;
}

void cfg_split_cleanup(cfg_split_t *split) {
  for (int i = 0; i < split->count; i++)
    free(split->values[i]);
  free(split->values);
  free(split);
}
