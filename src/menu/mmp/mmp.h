#pragma once
#include "../../common/ini/ini.h"
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
// PROTIP: don't change the sorting while iterating over the files.
#define G2HR_MMP_DEFAULT_SORT_SECTION "MapFiles"
#define G2HR_MMP_DEFAULT_SORT_KEY "Description"

typedef struct mmp_file_t {
  ini_t *data;
  char *source;
} mmp_file_t;

typedef struct mmp_t {
  mmp_file_t **files;
  size_t file_count;
} mmp_t;

mmp_t *mmp_init(char *path);
void mmp_sort(mmp_t *mmp, char *section, char *key);

void mmp_cleanup(mmp_t *mmp);
