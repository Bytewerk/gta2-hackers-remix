#include "mmp.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test mmp_folder\n");
  mmp_t *mmp = mmp_init(argv[1]);

  for (size_t i = 0; i < mmp->file_count; i++) {
    mmp_file_t *file = mmp->files[i];
    printf("%s (%s)\n", ini_read(file->data, "MapFiles", "Description"),
           file->data->fullpath);
  }

  mmp_cleanup(mmp);
}
