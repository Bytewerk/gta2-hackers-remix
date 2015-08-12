#include "../../src-3rdparty/bsdiff/bsdiff.h"
#include "../../src-3rdparty/bsdiff/bspatch.h"
#include "../../src/common/fs/fs.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: also create a header file, that contains the patched
// file size in src/common/vike_patch.h or something like that.

// returns 0 on success
int write(bsdiff_stream *stream, const void *buffer, int size) {
  fwrite(buffer, size, 1, (FILE *)stream->opaque);
  return 0;
}

// returns 0 on success
int read(const bspatch_stream *stream, void *buffer, int size) {
  fread(buffer, size, 1, (FILE *)stream->opaque);
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 5)
    exit(printf("Syntax: %s gta2.exe gta2_vike.exe"
                " patch_output patched_file_for_verification.exe\n",
                argv[0]));

  // load the input files
  uint32_t size_old;
  uint32_t size_new;

  char *buffer_old = fs_load_small_file_to_ram(argv[1], &size_old, 0);
  char *buffer_new = fs_load_small_file_to_ram(argv[2], &size_new, 0);

  printf("creating the patchfile...\n");
  FILE *handle = fopen(argv[3], "wb");
  if (!handle)
    exit(printf("ERROR: failed to open the file!\n"));

  bsdiff_stream stream;
  stream.opaque = (void *)handle;
  stream.malloc = malloc;
  stream.free = free;
  stream.write = write;

  bsdiff((const uint8_t *)buffer_old, size_old, (const uint8_t *)buffer_new,
         size_new, &stream);
  fclose(handle);

  printf("applying the patch...\n");
  handle = fopen(argv[3], "rb");
  char *buffer_patched = calloc(1, size_new);

  bspatch_stream stream2;
  stream2.opaque = (void *)handle;
  stream2.read = read;

  bspatch((const uint8_t *)buffer_old, size_old, (uint8_t *)buffer_patched,
          size_new, &stream2);
  fclose(handle);

  printf("writing %s...\n", argv[4]);
  handle = fopen(argv[4], "wb");
  fwrite(buffer_patched, size_new, 1, handle);
  fclose(handle);

  printf("cleaning up...\n");

  free(buffer_patched);
  free(buffer_old);
  free(buffer_new);
}
