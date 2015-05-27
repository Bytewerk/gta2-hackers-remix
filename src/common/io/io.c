#include "io.h"
#include <dirent.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read small files (<<10 MB) into RAM
// returned buffer must be free'd after usage!
char *io_load_small_file_to_ram(char *filename, uint16_t *size, char quiet) {
  if (!quiet)
    printf("loading %s...\n", filename);

  FILE *handle = fopen(filename, "rb");
  if (!handle)
    exit(printf("ERROR: opening file '%s' has failed!\n", filename));

  fseek(handle, 0, SEEK_END);
  *size = ftell(handle);
  rewind(handle);

  char *buffer = malloc(*size);
  rewind(handle);
  if (fread(buffer, 1, *size, handle) != *size)
    exit(printf("ERROR: reading '%s' has failed!\n", filename));
  fclose(handle);

  return buffer;
}

// callback syntax: func(char* filename, void* userdata)
// ext can also be NULL to iterate over all files in a folder
void io_iterate_over_files_in_folder(char *folder, char *ext, void *callback,
                                     void *userdata, char quiet) {
  if (!quiet)
    printf("loading %s/*.%s...\n", folder, ext);
  DIR *dir = opendir(folder);
  if (!dir)
    exit(printf("ERROR: couldn't find path '%s'!\n", folder));

  uint16_t len_folder = strlen(folder);
  uint16_t len_ext = ext ? strlen(ext) : 0;

  while (1) {
    // grab the next file in the folder
    struct dirent *entry = readdir(dir);
    if (!entry)
      break;

    // check if the file has the specified extension
    char *name = entry->d_name;
    uint16_t len_name = strlen(name);
    uint16_t pos_dot = len_name - len_ext - 1;
    if (ext && (len_name < len_ext || name[pos_dot] != '.' ||
                strcmp(name + pos_dot + 1, ext)))
      continue;

    // generate the full path
    uint16_t len_fullpath = len_folder + len_name + 1;
    char *fullpath = malloc(len_fullpath + 1);
    snprintf(fullpath, len_fullpath + 1, "%s/%s", folder, name);

    // execute the callback
    void (*func)(char *filename, void *userdata) = callback;
    func(fullpath, userdata);
    free(fullpath);
  }
  closedir(dir);
}
