/*
        Documentation from here:
        http://www.gtamodding.com/index.php?title=GXT#GTA2_Format

        Header:
                GBL
                E					// language letter, eg.
   E for english
                100					// uint16_t version
   number, always 100
        TKEY Chunk:
                TKEY
                (size)				// uint32_t
                (all TKEY entires)
        TKEY Entry:
                (offset in TDAT)	// uint32_t
                (entry_name)		// char[8]
        TDAT Chunk:
                TDAT
                (size)				// uint32_t
                (all text strings)
        text string:
                text				// format: char, 0x00, next
   char, ...
                                                        // sometimes contains
   some style info
                                                        // at the start
*/

#include "gxt.h"
#include "../../common/fs/fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GXT_STRING_BUFFER_SIZE 10000

// get string from gxt by ID
char *gxt_get(gxt_t *gxt, char *id) {
  gxt_entry_t *entry = gxt->entries;
  while (entry) {
    if (!strcmp(entry->id, id))
      return entry->text;
    entry = entry->next;
  }

  printf("ERROR: Couldn't find text for ID %s!\n", id);
  return "TEXT NOT FOUND";
}

// tkey, tday: buffers without the file and chunk headers
void gxt_parse(gxt_t *gxt, char *tkey, char *tdat, uint32_t tkey_size,
               uint32_t tdat_size) {
  char *text_buffer = malloc(GXT_STRING_BUFFER_SIZE);

  gxt_entry_t *last = NULL;
  uint32_t tkey_offset = 0;
  while (tkey_offset < tkey_size) {
    // create a new entry
    gxt_entry_t *entry = malloc(sizeof(gxt_entry_t));
    entry->next = NULL;

    // save ID
    char *id = (char *)(tkey + tkey_offset + 4);
    int id_size = strlen(id);
    if (id_size > 8)
      exit(printf("ERROR: invalid ID size: %i\n", id_size));
    entry->id = malloc(id_size + 1);
    entry->id[id_size] = '\0';
    strncpy(entry->id, id, id_size);

    // save actual text
    uint32_t tdat_offset = *(uint32_t *)(tkey + tkey_offset);
    for (int i = 0;; i++) {
      if (i > GXT_STRING_BUFFER_SIZE)
        exit(printf(
            "ERROR: String %s is larger than max. string buffer (%i of %i)!\n",
            entry->id, i, GXT_STRING_BUFFER_SIZE));
      char c = tdat[tdat_offset + i * 2];
      text_buffer[i] = c;
      if (!c)
        break;
    }
    int text_size = strlen(text_buffer);
    entry->text = malloc(text_size + 1);
    entry->text[text_size] = '\0';
    strncpy(entry->text, text_buffer, text_size);

    // attach to the list
    if (last)
      last->next = entry;
    else
      gxt->entries = entry;
    last = entry;

    tkey_offset += 12;
  }

  free(text_buffer);
}

gxt_t *gxt_load(char *filename) {
  uint32_t size;
  char *buffer = fs_load_small_file_to_ram(filename, &size, 0);
  gxt_t *gxt = malloc(sizeof(gxt_t));

  // check file header ("GBL", lang, 0x6400)
  if (buffer[0] != 'G' || buffer[1] != 'B' || buffer[2] != 'L' ||
      buffer[4] != 0x64 || buffer[5])
    exit(printf("ERROR: this isn't a valid GTA2 GXT file!"));
  gxt->lang = buffer[3];

  // check the TKEY chunk header
  if (buffer[6] != 'T' || buffer[7] != 'K' || buffer[8] != 'E' ||
      buffer[9] != 'Y')
    exit(printf("ERROR: expected TKEY as first chunk!\n"));
  uint32_t tkey_size = *(uint32_t *)(buffer + 10);

  // check the TDAT chunk header
  char *tdatbuf = 6 /* file header */ + 8 /* chunk header */
                  + tkey_size + buffer;
  if (tdatbuf[0] != 'T' || tdatbuf[1] != 'D' || tdatbuf[2] != 'A' ||
      tdatbuf[3] != 'T')
    exit(printf("ERROR: couldn't find TDAT chunk after TKEY!\n"));
  uint32_t tdat_size = *(uint32_t *)(tdatbuf + 4);

  gxt_parse(gxt, buffer + 6 /*file header*/ + 8 /*chunk header*/,
            tdatbuf + 8 /*chunk header*/, tkey_size, tdat_size);

  // done
  free(buffer);
  return gxt;
}

void gxt_cleanup(gxt_t *gxt) {
  gxt_entry_t *next = gxt->entries;
  while (next) {
    gxt_entry_t *old = next;
    next = next->next;
    free(old->id);
    free(old->text);
    free(old);
  }
  free(gxt);
}
