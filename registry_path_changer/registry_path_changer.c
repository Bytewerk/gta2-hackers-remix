#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace(char *buffer, int *i, char *original, char *modded) {
  int size = strlen(original);
  int j = 0;
  int k;

  while (buffer[*i + j] == original[j]) {
    if (j < size - 1)
      j++;
    else {
      printf("replaced string successfully!\n");
      for (k = 0; k < size; k++) {
        buffer[*i + k] = modded[k];
      }
      *i += size;
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *handle;
  char *buffer;
  int size;
  int i;

  // String we will replace, must be same size (21 characters)!
  // the 1 in P1 will be the player number.
  char original[] = "\\DMA Design Ltd\\GTA2\\";
  char modded[] = "\\GTA2HackersRemix\\Pi\\";

  if (argc != 4) {
    printf("---------------------------------------------------------------\n");
    printf("      GTA2 Hackers Remix: Registry Path Changer v0.2\n");
    printf("      https://github.com/Bytewerk/gta2-hackers-remix\n");
    printf("---------------------------------------------------------------\n");
    printf(" Syntax:  registry_path_changer.exe PLAYER_NUMBER ORIG DEST\n");
    printf(" Example: registry_path_changer.exe 1 c:\\gta2.exe c:\\mod.exe\n");
    printf("---------------------------------------------------------------\n");
    printf(" This program replaces all (registry) strings that start with:\n");
    printf("     %s\n", original);
    printf(" with the following (i will be replaced with PLAYER_NUMBER):\n");
    printf("     %s\n", modded);
    printf("---------------------------------------------------------------\n");
    return 1;
  };

  // Write the ID to the modded string
  modded[19] = argv[1][0];

  // Open GTA2.exe
  handle = fopen(argv[2], "rb");
  if (handle == NULL)
    return printf("Couldn't read file \"%s\"!\n", argv[2]);

  // Find out the size
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // Read the whole GTA2.exe file into RAM and close it
  rewind(handle);
  buffer = (char *)malloc(sizeof(char) * size);
  if (fread(buffer, 1, size, handle) != size)
    return printf("read error!\n");
  fclose(handle);

  // Replace all instances of the original string
  // with the modded one in the buffer
  for (i = 0; i < size; i++)
    replace(buffer, &i, original, modded);

  // write everything to the target file
  handle = fopen(argv[3], "w+b");
  if (handle == NULL)
    return printf("Couldn't write file \"%s\"!\n", argv[3]);
  fwrite(buffer, 1, size, handle);
  fclose(handle);

  printf("done!\n");
  return 0;
};
