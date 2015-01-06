#include "sty_parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* offsets in fstyle.sty, found with the code below
    Chunk: PPAL - Size: 0065536 - Delta: 0000006 - Offset: 0000006
    Chunk: SPRB - Size: 0000012 - Delta: 0065544 - Offset: 0065550
    Chunk: FONB - Size: 0000030 - Delta: 0000020 - Offset: 0065570
    Chunk: PALX - Size: 0032768 - Delta: 0000038 - Offset: 0065608
    Chunk: SPRG - Size: 0655360 - Delta: 0032776 - Offset: 0098384
    Chunk: SPRX - Size: 0014128 - Delta: 0655368 - Offset: 0753752
    Chunk: PALB - Size: 0000016 - Delta: 0014136 - Offset: 0767888
*/

/*
void sty_parser_find_chunks(char* buffer, uint32_t size)
{
    const char *GTA2_STY_CHUNK_NAMES [] =
        {"PALX", "PPAL","PALB","TILE",
        "SPRG","SPRX","SPRB","DELS","DELX","FONB","CARI",
        "OBJI","PSXT","RECY"};
    uint32_t last = 0;

    for(uint32_t i=0;i<size;i++)
    {
        for(int j=0;j<14;j++)
        {
            if(   buffer[i+0] != GTA2_STY_CHUNK_NAMES[j][0]
               || buffer[i+1] != GTA2_STY_CHUNK_NAMES[j][1]
               || buffer[i+2] != GTA2_STY_CHUNK_NAMES[j][2]
               || buffer[i+3] != GTA2_STY_CHUNK_NAMES[j][3]
            ) continue;

            uint32_t chunk_size = *(uint32_t*) (buffer + i + 4);

            printf("Chunk: %c%c%c%c ", buffer[i], buffer[i+1],
                buffer[i+2], buffer[i+3]);
            printf("- Size: %07i ", chunk_size);
            printf("- Delta: %07i ", i -last);
            printf("- Offset: %07i ", i);
            printf("\n");
            last = i;
        }
    }
}

*/

void sty_parser_read_font_base(sty_t *sty, char *buffer_pos, uint32_t length,
                               char *type) {
  if (strcmp("FONB", type))
    return;

  printf("\tFONB chunk\n");

  // get the font_count and check if it makes sense
  uint16_t font_count = *(uint16_t *)buffer_pos;
  if (font_count * 2 + 2 != length)
    exit(printf("ERROR: font_count doesn't match chunk size!\n"));

  // save the font base
  uint16_t *base = malloc(2 * font_count);
  for (int i = 0; i < font_count; i++)
    base[i] = *(uint16_t *)(buffer_pos + 2 + 2 * i);

  sty->font_base->font_count = font_count;
  sty->font_base->base = base;
}

// returns the next offset or 0
uint32_t sty_parser_read_next_chunk(sty_t *sty, char *buffer, uint32_t offset,
                                    uint32_t sty_size) {
  // read the chunk type and size
  char type[5];
  for (int i = 0; i < 4; i++)
    type[i] = buffer[offset + i];
  type[5] = '\0';
  uint32_t chunk_size = *(uint32_t *)(buffer + offset + 4);

  // check if the chunk size makes sense
  if (!chunk_size || offset >= sty_size)
    exit(printf("ERROR!\n"));

  // read the chunk content - whatever fits
  sty_parser_read_font_base(sty, buffer + offset + 8, chunk_size, type);

  // return the new offset (or 0 if we're done here)
  offset += chunk_size + 8 /* chunk header */;
  if (offset == sty_size)
    return 0;
  return offset;
}

sty_t *sty_parser(char *filename) {
  printf("sty_parser: reading file %s\n", filename);

  // open the file
  FILE *handle;
  handle = fopen(filename, "rb");
  if (handle == NULL)
    exit(printf("Can't open the file!\n"));

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(sizeof(char) * size);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error!\n"));
  fclose(handle);

  // check the file header
  if (buffer[0] != 'G' || buffer[1] != 'B' || buffer[2] != 'S' ||
      buffer[3] != 'T')
    exit(printf("This isn't a GBST file!\n"));

  // create an empty sty structure
  sty_t *sty = malloc(sizeof(sty_t));
  sty->font_base = malloc(sizeof(font_base_t));
  sty->font_base->font_count = 0;

  // fill the sty
  uint32_t offset = 6; // skip the header!
  while (offset)
    offset = sty_parser_read_next_chunk(sty, buffer, offset, size);

  free(buffer);
  return sty;
}

void sty_cleanup(sty_t *sty) {
  if (sty->font_base->font_count)
    free(sty->font_base->base);
  free(sty);
}
