#pragma once
#include <sys/types.h>

/*
        See also elypter's thread "The .mmp file standard":
                http://gtamp.com/forum/viewtopic.php?t=188

        MMP-file contents:
                [MapFiles]
                GMPFile=test.gmp
                Description=test
                ...

        Reading MMP files:
                mmp_t* mmp = mmp_init("GTA2/data");
                for(size_t i=0; i < mmp->file_count; i++)
                        mmp_read(mmp->files[i], "MapFiles/Description");
                mmp_cleanup(mmp);
*/

// This is the key that all files get sorted by in the init function.
// If you want to have another sorting, make use of mmp_sort(mmp, key).
// Pro-tip: don't change the sorting while iterating over the files.
#define G2HR_MMP_DEFAULT_SORT_KEY "MapFiles/Description"

typedef struct mmp_key_t {
  struct mmp_key_t *next;
  char *key;
  char *value;
} mmp_key_t;

typedef struct mmp_file_t {
  mmp_key_t *data;
  char *source;
} mmp_file_t;

typedef struct mmp_t {
  mmp_file_t **files;
  size_t file_count;
} mmp_t;

// temporary list format, that only gets used while parsing the
// directory. after that, an array gets created (so it can be sorted
// with qsort!)
typedef struct mmp_list_t {
  struct mmp_list_t *next;
  mmp_file_t *file;
} mmp_list_t;

mmp_t *mmp_init(const char *path);
void mmp_sort(mmp_t *mmp, char *key);
char *mmp_read(mmp_file_t *file, char *key);
void mmp_cleanup(mmp_t *mmp);
