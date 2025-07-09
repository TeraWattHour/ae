#pragma once

#include <stddef.h>

typedef enum token_kind_t {
  tok_eof = 0,
  tok_plus,
  tok_plus_equals,
  tok_minus,
  tok_identifier,
  tok_integer,
} token_kind_t;

typedef struct token_t {
  char *start;
  char *end;
  token_kind_t kind;
} token_t;

typedef struct lexer_t {
  char *content;
  const char *file;

  size_t line;
  size_t column;

  char *c;
  char *s;

  token_t token;
} lexer_t;

typedef struct location_t {
  size_t line;
  size_t column;
  char *file;
} location_t;

lexer_t lexer_init(char *file, char *source);
int lexer_next_token(lexer_t *lexer);
void lexer_skip_whitespace(lexer_t *lexer);
location_t lexer_location(lexer_t *lexer, char *start);