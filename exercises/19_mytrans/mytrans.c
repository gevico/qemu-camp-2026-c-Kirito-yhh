// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *start = str;
    char *end;
    
    while (*start == ' ' || *start == '\t')
        start++;
    
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        end--;
    
    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    trim(line);
    
    if (line[0] == '#') {
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      in_entry = 1;
    } else if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1] = '\0';
      
      hash_table_insert(table, current_word, current_translation);
      (*dict_count)++;
      
      in_entry = 0;
    }
  }

  fclose(file);
  return 0;
}
