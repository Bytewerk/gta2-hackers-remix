#pragma once
#include <inttypes.h>

// read small files (<<10 MB) into RAM
// returned buffer must be free'd after usage!
char *io_load_small_file_to_ram(char *filename, uint16_t *size, char quiet);

// callback syntax: func(char* filename, void* userdata)
// ext can also be NULL to iterate over all files in a folder
void io_iterate_over_files_in_folder(char *folder, char *ext, void *callback,
                                     void *userdata, char quiet);
