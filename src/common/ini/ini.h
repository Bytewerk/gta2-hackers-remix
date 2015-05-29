#pragma once
#include <stdbool.h>

typedef struct ini_entry_t {
  struct ini_entry_t *next;

  char *key;
  char *value;

} ini_entry_t;

typedef struct ini_section_t {
  struct ini_section_t *next;
  ini_entry_t *entries;
  char *name;

} ini_section_t;

typedef struct { ini_section_t *sections; } ini_t;

// if fullpath is NULL or the file can not be read, an empty ini_t
// struct gets initialized.
ini_t *ini_open(char *fullpath, bool quiet);

char *ini_read(ini_t *ini, char *section, char *key);

char *ini_modify(ini_t *ini, char *section, char *key, bool overwrite);

void ini_save(ini_t *ini, char *fullpath, bool quiet);

void ini_cleanup(ini_t *ini);
