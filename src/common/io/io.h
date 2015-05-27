#pragma once
#include <inttypes.h>

// read small files (<<10 MB) into RAM
// returned buffer must be free'd after usage!
char *io_load_small_file_to_ram(char *filename, uint32_t *size, char quiet);

// the returned substring is the end of the filename, and gets free'd
// when filename gets free'd!
char *io_get_file_ext(char *filename);

// ext may be NULL. use io_get_file_ext to get it, if necessary.
// the return value must be free'd after usage!
char *io_get_filename_without_ext(char *filename, char *ext);

// callback args: (char* path, char* name, char* ext, void* userdata)
// if you want to store a char*-parameter in the userdata, you must copy
// its contents!
//
// ext can also be NULL to iterate over all files in a folder
void io_iterate_over_files_in_folder(char *folder, char *ext, void *callback,
                                     void *userdata, char quiet);
