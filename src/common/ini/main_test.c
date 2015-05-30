#include "ini.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return printf("Syntax: main_test ini_file\n");

  ini_t *ini = ini_open(argv[1], true, false);

  // set a default values
  ini_modify(ini, "slotmachine", "enabled", "false", false);
  ini_modify(ini, "new section", "new value", "default value", false);
  ini_modify(ini, "multiplayer", "cops_enabled", "true", false);

  // overwrite a value
  ini_modify(ini, "slotmachine", "cmd_reboot", "overwritten!", true);

  printf("full dump:\n");
  ini_section_t *section = ini->sections;
  while (section) {
    printf("\t'%s':\n", section->name);
    ini_entry_t *entry = section->entries;
    while (entry) {
      printf("\t\t'%s': '%s'\n", entry->key, entry->value);
      entry = entry->next;
    }
    section = section->next;
  }

  printf("only accessing slotmachine/enabled:\n");
  printf("\t'%s'\n", ini_read(ini, "slotmachine", "enabled"));

  ini_save(ini, "test_output_file.ini", true, false);

  printf("cleaning up...\n");
  ini_cleanup(ini);
  printf("done!\n");
}
