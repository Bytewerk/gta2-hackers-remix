#include "mmp.h"
#include <stdio.h>
#include <stdlib.h>

mmp_t *mmp_init() {
  // TODO: read all mmp files in data folder
  return NULL;
}

mmp_t *mmp_load(char *filename) {
  mmp_t *mmp = malloc(sizeof(mmp_t));
  printf("loading %s...\n", filename);
  mmp->source = filename;
  mmp->keys = NULL;
  mmp->next = NULL;

  FILE *handle = fopen(filename, "rb");
  if (!handle)
    exit(printf("ERROR: Couldn't read file!"));

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(size);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error!"));
  fclose(handle);

  free(buffer);
  return mmp;
}

char *mmp_read(mmp_t *mmp, char *key) { return "TODO"; }

void mmp_cleanup(mmp_t *mmp) {
  while (mmp) {
    // free all keys
    mmp_key_t *keys = mmp->keys;
    while (keys) {
      mmp_key_t *old = keys;
      keys = keys->next;
      free(old->key);
      free(old->value);
      free(old);
    }

    // free the mmp struct itself
    mmp_t *old = mmp;
    mmp = mmp->next;
    free(old);
  }
}
