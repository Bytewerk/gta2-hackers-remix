#include "io.h"
#include <dirent.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read small files (<<10 MB) into RAM
// returned buffer must be free'd after usage!
char *io_load_small_file_to_ram(char *filename, uint32_t *size, char quiet) {
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

// the returned substring is the end of the filename, and gets free'd
// when filename gets free'd!
char *io_get_file_ext(char *filename) {
  uint16_t len = strlen(filename);
  char *pos = strrchr(filename, '.');
  if (!pos || pos - filename > len)
    return NULL;
  return pos + 1;
}

// ext may be NULL. use io_get_file_ext to get it, if necessary.
// the return value must be free'd after usage!
char *io_get_filename_without_ext(char *filename, char *ext) {
  uint16_t len = strlen(filename);

  if (ext)
    len -= strlen(ext) + 1;

  char *ret = malloc(len + 1);
  strncpy(ret, filename, len);
  ret[len] = '\0';
  return ret;
}

// callback args: (char* path, char* name, char* ext, void* userdata)
// if you want to store a char*-parameter in the userdata, you must copy
// its contents!
//
// ext can also be NULL to iterate over all files in a folder
void io_iterate_over_files_in_folder(char *folder, char *ext, void *callback,
                                     void *userdata, char quiet) {
  if (!quiet)
    printf("loading %s/*.%s...\n", folder, ext);
  DIR *dir = opendir(folder);
  if (!dir)
    exit(printf("ERROR: couldn't find path '%s'!\n", folder));

  uint16_t len_folder = strlen(folder);

  while (1) {
    // grab the next file in the folder
    struct dirent *entry = readdir(dir);
    if (!entry)
      break;

    // check if the file has the specified extension
    char *file_name = entry->d_name;
    uint16_t len_name = strlen(file_name);
    char *file_ext = io_get_file_ext(file_name);

    // skip if the extension is not the one we want
    if (ext && (!file_ext || strcmp(file_ext, ext)))
      continue;

    // generate the full path
    uint16_t len_fullpath = len_folder + len_name + 1;
    char *file_path = malloc(len_fullpath + 1);
    snprintf(file_path, len_fullpath + 1, "%s/%s", folder, file_name);

    // execute the callback
    void (*func)(char *path, char *name, char *ext, void *userdata) = callback;
    func(file_path, file_name, file_ext, userdata);
    free(file_path);
  }
  closedir(dir);
}
