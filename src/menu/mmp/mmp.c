#include "mmp.h"
#include "../../common/cfg/cfg.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// qsort_r isn't in C99, so we'll just store the sorting key globally.
char *GLOBAL_SORT_KEY = G2HR_MMP_DEFAULT_SORT_KEY;

int mmp_comparator(const void *a, const void *b) {
  return strcmp(cfg_read(((mmp_file_t *)a)->data, GLOBAL_SORT_KEY),
                cfg_read(((mmp_file_t *)b)->data, GLOBAL_SORT_KEY));
}

void mmp_sort(mmp_t *mmp, char *key) {
  GLOBAL_SORT_KEY = key;
  qsort(mmp->files, mmp->file_count, sizeof(mmp_file_t *), mmp_comparator);
}

mmp_file_t *mmp_load(char *filename) {
  mmp_file_t *file = malloc(sizeof(mmp_file_t));
  file->source = filename;
  file->data = cfg_load(filename, 1);
  return file;
}

// load all files in the data/ folder and sort them by the
// description field (which is something like the display title).
mmp_t *mmp_init(const char *path) {
  printf("loading %s/*.mmp...\n", path);
  DIR *dir = opendir(path);
  if (!dir)
    exit(printf("Couldn't find path '%s'!\n", path));

  // struct that we'll return
  mmp_t *mmp = malloc(sizeof(mmp_t));
  mmp->file_count = 0;

  // temporary list structs
  mmp_list_t *first = NULL;
  mmp_list_t *current = NULL;

  size_t len_path = strlen(path);
  while (1) {
    struct dirent *entry = readdir(dir);
    if (!entry)
      break;

    // skip non-.mmp-files
    char *name = entry->d_name;
    size_t len = strlen(name);
    if (len < 4 || name[len - 4] != '.' ||
        (name[len - 3] != 'm' && name[len - 3] != 'M') ||
        (name[len - 2] != 'm' && name[len - 2] != 'M') ||
        (name[len - 1] != 'p' && name[len - 1] != 'P'))
      continue;

    // load the file (the full path gets free'd on cleanup!)
    char *fullpath = malloc(len_path + len + 2);
    snprintf(fullpath, len_path + len + 2, "%s/%s", path, name);
    mmp_list_t *new = malloc(sizeof(mmp_list_t));
    new->next = NULL;
    new->file = mmp_load(fullpath);

    // add it to the temporary list
    if (first)
      current->next = new;
    else
      first = new;
    current = new;
    mmp->file_count++;
  }
  closedir(dir);

  // transform the list into an array
  mmp->files = malloc(sizeof(mmp_file_t *) * mmp->file_count);
  current = first;
  for (size_t i = 0; i < mmp->file_count; i++) {
    first = current;
    mmp->files[i] = first->file;
    current = first->next;
    free(first);
  }

  // sort the maps by the description field
  mmp_sort(mmp, G2HR_MMP_DEFAULT_SORT_KEY);
  return mmp;
}

void mmp_cleanup(mmp_t *mmp) {
  for (size_t i = 0; i < mmp->file_count; i++) {
    mmp_file_t *file = mmp->files[i];
    cfg_cleanup(file->data);
    free(file->source);
    free(file);
  }
  free(mmp->files);
  free(mmp);
}
