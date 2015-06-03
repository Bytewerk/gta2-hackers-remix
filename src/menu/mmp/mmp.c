#include "mmp.h"
#include "../../common/cstr/cstr.h"
#include "../../common/io/io.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// qsort_r isn't in C99, so we'll just store the sorting key and section
// globally.
char *GLOBAL_MMP_SORT_SECTION;
char *GLOBAL_MMP_SORT_KEY;

int mmp_comparator(const void *a, const void *b) {

  char *one = ini_read((*(mmp_file_t **)a)->data, GLOBAL_MMP_SORT_SECTION,
                       GLOBAL_MMP_SORT_KEY);
  char *two = ini_read((*(mmp_file_t **)b)->data, GLOBAL_MMP_SORT_SECTION,
                       GLOBAL_MMP_SORT_KEY);

  if (!one || !two)
    printf("ERROR: file %s doesn't contain key %s in section %s!\n",
           (*(mmp_file_t **)a)->source, GLOBAL_MMP_SORT_SECTION,
           GLOBAL_MMP_SORT_KEY);

  if (!one)
    one = "";
  if (!two)
    two = "";

  return strcmp(one, two);
}

void mmp_sort(mmp_t *mmp, char *section, char *key) {
  GLOBAL_MMP_SORT_KEY = key;
  GLOBAL_MMP_SORT_SECTION = section;

  qsort(mmp->files, mmp->file_count, sizeof(mmp_file_t *), mmp_comparator);
}

// temporary list format, that only gets used while parsing the
// directory. after that, an array gets created (so it can be sorted
// with qsort!)
typedef struct mmp_list_t {
  struct mmp_list_t *next;
  mmp_file_t *file;
} mmp_list_t;

void mmp_load(char *path, char *name, char *ext, void *userdata) {
  mmp_list_t *new = malloc(sizeof(mmp_list_t));
  new->file = malloc(sizeof(mmp_file_t));
  new->file->data = ini_open(path, true, true);
  new->file->source = cstr_copy(name);

  // attach it to the head of the temporary list (gets sorted later)
  new->next = *((mmp_list_t **)userdata);
  *((mmp_list_t **)userdata) = new;
}

// load all files in the data/ folder and sort them by the
// description field (which is something like the display title).
mmp_t *mmp_init(char *path) {
  // read all files into a temporary list
  mmp_list_t *list_temp = NULL;
  io_iterate_over_files_in_folder(path, "mmp", mmp_load, &list_temp, false);

  // struct that we'll return
  mmp_t *mmp = malloc(sizeof(mmp_t));
  mmp->file_count = 0;

  // count the list entries
  mmp_list_t *current = list_temp;
  while (current) {
    mmp->file_count++;
    current = current->next;
  }

  // transform the list into an array
  mmp->files = malloc(sizeof(mmp_file_t *) * mmp->file_count);
  current = list_temp;
  for (size_t i = 0; i < mmp->file_count; i++) {
    mmp->files[i] = current->file;

    mmp_list_t *old = current;
    current = current->next;
    free(old);
  }

  // sort the maps by the description field
  mmp_sort(mmp, G2HR_MMP_DEFAULT_SORT_SECTION, G2HR_MMP_DEFAULT_SORT_KEY);
  return mmp;
}

void mmp_cleanup(mmp_t *mmp) {
  for (size_t i = 0; i < mmp->file_count; i++) {
    mmp_file_t *file = mmp->files[i];
    ini_cleanup(file->data);
    free(file->source);
    free(file);
  }
  free(mmp->files);
  free(mmp);
}
