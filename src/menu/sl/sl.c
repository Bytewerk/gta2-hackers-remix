#include "sl.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
        sl_parse() is a port from AutoIt3 to C99 of the function
        screen_layout_init() of screen_layout_init.au3, which was written
        for the alpha version of the splitscreen mod.
*/

#define ERR(MSG)                                                               \
  exit(printf("ERROR (line %i, col %i, char '%c'): %s!\n", line_number,        \
              char_in_line, c, MSG))

// checks if there is a hole in the block_geometry array,
// eg. if player 2 was forgotten, but 1 and 3 are set
// returns the player count, from 0 to 5
int sl_parse_check_block(int line_number, int char_in_line, char c,
                         sl_geo_t **block_geometry) {
  int last_found = 0;
  for (int i = 1; i < GTA2_MAX_PLAYERS; i++)
    if (block_geometry[i]) {
      if (last_found != i - 1)
        ERR("missing player number in layout block (eg. 1,3 are set, but 2 is "
            "missing)");
      last_found = i;
    }

  if (!block_geometry[0])
    ERR("player 1 is missing in this layout block!");

  return last_found;
}

// returns an array of an entry list
sl_entry_t **sl_parse(char *buffer, size_t buffer_size) {
  sl_entry_t **entries = calloc(1, sizeof(sl_entry_t *) * GTA2_MAX_PLAYERS);
  sl_geo_t **block_geometry = NULL;
  char is_comment_line = 0;
  int char_in_line = 0;
  int line_number = 1;
  int layout_width = 0;
  int layout_height = 0;
  int last_left_border_pos_in_line = -1; // -1: not found in line yet

  for (size_t i = 0; i < buffer_size; i++) {
    char c = buffer[i];

    if (block_geometry && !layout_width && c != '\n' && c != '\r' && c != '-')
      ERR("The first line of a new layout block must only consist of dashes "
          "(-)");

    // beginning of a new line
    if (char_in_line == 0) {
      // layout block start
      if (c == '-' && !block_geometry) {
        block_geometry = calloc(1, sizeof(sl_geo_t *) * GTA2_MAX_PLAYERS);
        layout_width = 0;
        layout_height = 0;
        is_comment_line = 0;
      }
      // layout block end ('\n\n')
      else if (c == '\n' && block_geometry) {
        int player_count =
            sl_parse_check_block(line_number, char_in_line, c, block_geometry);

        // Explanation for the '-1' in w and h:
        // we count the top left edge, but not the bottom right
        // one.
        sl_entry_t *entry = malloc(sizeof(sl_entry_t));
        entry->w = layout_width - 1;
        entry->h = layout_height - 1;
        entry->geo = block_geometry;
        entry->next = NULL;

        if (entries[player_count]) {
          sl_entry_t *current = entries[player_count];
          while (current->next)
            current = current->next;
          current->next = entry;
        } else
          entries[player_count] = entry;

        block_geometry = NULL;
        last_left_border_pos_in_line = -1;
      } else if (c != '-' && c != '|') {
        if (block_geometry)
          ERR("Found a comment line inside a layout block. This isn't allowed. "
              "Make sure that you have an empty line after each layout block");
        if (c != '\n' && c != '\r' && c != '#')
          ERR("All comment lines must begin with a hash sign (#)");
        is_comment_line = 1;
      }
    }

    if (!is_comment_line) {
      if (c == '|')
        last_left_border_pos_in_line = char_in_line;

      // We found a number! let's measure the geometry by going
      // left, right, up and down until we hit all the borders.
      else if (c >= '1' && c <= '0' + GTA2_MAX_PLAYERS) {
        if (block_geometry[c - '1'])
          ERR("You must not use the same player number in one screen layout "
              "more than once");

        if (last_left_border_pos_in_line == -1)
          ERR("No border ('|') to the left of this number");

        sl_geo_t *geo = calloc(1, sizeof(sl_geo_t));

        // left
        geo->x = last_left_border_pos_in_line;

        // right
        for (int j = 0; i + j <= buffer_size; j++) {
          if (i + j == buffer_size || buffer[i + j] == '\n' ||
              buffer[i + j] == '\r')
            ERR("No border ('|') to the right of this number");
          if (buffer[i + j] != '|')
            continue;
          geo->w = char_in_line + j - last_left_border_pos_in_line;
          break;
        }

        // up
        for (int j = 0;; j++) {
          if (buffer[i - j * layout_width] != '-')
            continue;
          geo->y = layout_height - j;
          break;
        }

        // down
        // FIXME: check at \n if bottom line was -----
        for (int j = 0; i + j < buffer_size; j += layout_width)
          if (buffer[i + j] == '-') {
            geo->h = -1 * (geo->y - layout_height) + j / layout_width;
            break;
          }
        if (!geo->h)
          ERR("No border ('-') at the bottom of this number");

        block_geometry[c - '1'] = geo;
      }

      else if (c > '0' + GTA2_MAX_PLAYERS)
        ERR("Invalid number (1...6 allowed)");
      else if (c != ' ' && c != '-' && c != '\n' && c != '\r')
        ERR("Invalid symbol");
    }

    if (c == '\n') {
      if (block_geometry) {
        if (!layout_width)
          layout_width = char_in_line;
        else if (layout_width != char_in_line)
          ERR("All lines in a layout block must have the same width!");

        layout_height++;
      }
      char_in_line = 0;
      line_number++;
    } else
      char_in_line++;
  }

  return entries;
}
#undef ERR

sl_t *sl_init(char *filename) {
  printf("loading %s...\n", filename);
  FILE *handle = fopen(filename, "rb");
  if (!handle)
    exit(printf("ERROR: Couldn't read '%s'!\n", filename));

  // calculate file size
  int size;
  fseek(handle, 0, SEEK_END);
  size = ftell(handle);

  // read the whole file into RAM (<<10 MB) and close it
  char *buffer;
  rewind(handle);
  buffer = (char *)malloc(size + 2);
  if (fread(buffer, 1, size, handle) != size)
    exit(printf("Read error while reading '%s'!\n", filename));
  fclose(handle);

  // add two additional new lines at the end for easier parsing
  buffer[size] = '\n';
  buffer[size + 1] = '\n';

  // parse the file
  sl_entry_t **entries = sl_parse(buffer, size + 2);
  free(buffer);

  // convert the list into an array for easier access
  sl_t *sl = malloc(sizeof(sl_t));
  for (int i = 0; i < GTA2_MAX_PLAYERS; i++) {
    // count the entries
    sl_player_t *player = malloc(sizeof(sl_player_t));
    sl_entry_t *entry = entries[i];
    uint16_t count = 0;
    while (entry) {
      count++;
      entry = entry->next;
    }

    // transform into an array
    player->count = count;
    player->layouts = malloc(sizeof(sl_entry_t *) * count);
    entry = entries[i];
    for (uint16_t j = 0; j < count; j++) {
      player->layouts[j] = entry;
      entry = entry->next;
    }
    sl->players[i] = player;
  }

  free(entries);

  return sl;
}

void sl_cleanup(sl_t *sl) {
  for (int i = 0; i < GTA2_MAX_PLAYERS; i++)
    if (sl->players[i]) {
      sl_player_t *player = sl->players[i];
      if (!player)
        continue;

      for (uint16_t j = 0; j < player->count; j++) {
        sl_entry_t *entry = player->layouts[j];
        for (int k = 0; k < GTA2_MAX_PLAYERS; k++)
          if (entry->geo[k])
            free(entry->geo[k]);
        free(entry->geo);
        free(entry);
      }
      free(player->layouts);
      free(player);
    }
  free(sl);
}
