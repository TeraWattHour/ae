#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(char *filename) {
  FILE* file = fopen(filename, "r");
  
  if (file == NULL) return NULL;
  
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);
  
  char* buffer = (char*)malloc(file_size + 1);
  if (buffer == NULL) { 
    fclose(file); 
    return NULL;
  }
  
  size_t bytes_read = fread(buffer, 1, file_size, file);
  if (bytes_read != file_size) {
    free(buffer);
    fclose(file);
    return NULL;
  }
    
  buffer[bytes_read] = '\0';
  return buffer;
}

int main() {
  char *source = read_file("test/main.ae");

  lexer_t lexer = lexer_init("test/main.vt", source);
  int ret = lexer_next_token(&lexer);
  while (ret > 0) {
    token_t token = lexer.token;
    printf("%d: %.*s\n", token.kind, token.end - token.start, token.start);
    ret = lexer_next_token(&lexer);
  }
}
