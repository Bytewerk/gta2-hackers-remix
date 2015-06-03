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

typedef struct {
  ini_section_t *sections;
  char *fullpath;

} ini_t;

// if fullpath is NULL or the file can not be read, an empty ini_t
// struct gets initialized. The fullpath pointer gets stored in the
// struct and free'd on cleanup. If you wish to store a copy instead,
// set copy_fullpath to true.
ini_t *ini_open(char *fullpath, bool copy_fullpath, bool quiet);

// returns NULL if the key was not found. If you need a default value,
// set it with ini_modify first.
char *ini_read(ini_t *ini, char *section, char *key);

// returns a reference to the new buffer that holds a copy of value.
// this buffer is part of the ini_t struct then.
char *ini_modify(ini_t *ini, char *section_name, char *key, char *value,
                 bool overwrite);

// pass NULL as fullpath to use the one specified in ini_open.
// protip writes a comment, that the file should not be modified by the
// user.
void ini_save(ini_t *ini, char *fullpath, bool noprotip, bool quiet);

void ini_cleanup(ini_t *ini);

// debug function for dumping ini contents with printf
void ini_dump(ini_t *ini);
