#include "lexer.h"
#include "parser.h"

#define DATASTRUCTURES_DA_IMPLEMENTATION
#include "dynamic_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

  lexer_t lexer = lexer_init("test/main.ae", source);
  token_t *tokens = da_alloc(token_t);
  bool lexing_failed = lexer_lex(&lexer, &tokens);

  printf("here: %d\n", da_len(tokens));

  for (int i = 0; i < da_len(tokens); i++) {
    printf("%d %.*s\n", tokens[i].kind, sv_print(tokens[i].content));
  }

  // parser_t parser = parser_init(&lexer);
  // statement_t *stmt = parser_next_statement(&parser);

  // printf("%.*s :: %.*s\n", sv_print(stmt->as.const_bind.rhs->as.identifier), sv_print(stmt->as.const_bind.lhs->as.identifier));
}
