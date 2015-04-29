#pragma once
#include <sys/types.h>

/*
        See also elypter's "The .mmp file standard":
                http://gtamp.com/forum/viewtopic.php?f=4&t=188

        MMP-file:
                [MapFiles]
                GMPFile=test.gmp
                Description=test

        Read:
                mmp_read(mmp, "MapFiles/Description");

*/

typedef struct mmp_key_t {
  struct mmp_key_t *next;
  char *key;
  char *value;
} mmp_key_t;

typedef struct mmp_file_t {
  mmp_key_t *data;
  char *source;
} mmp_file_t;

// temporary list format, that only gets used while parsing the
// directory. after that, an array gets created (so it can be sorted
// with qsort!)
typedef struct mmp_list_t {
  struct mmp_list_t *next;
  mmp_file_t *file;
} mmp_list_t;

typedef struct mmp_t {
  mmp_file_t **files;
  size_t file_count;
} mmp_t;

mmp_t *mmp_init(const char *path);
char *mmp_read(mmp_file_t *file, char *key);
void mmp_cleanup(mmp_t *mmp);
