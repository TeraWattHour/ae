#pragma once

#include "./sv.h"

#include <stddef.h>

typedef enum token_kind_t {
  tok_eof = 0,

  // tok_err means that the encountered character is completely unrecognizable,
  // a partially valid token may be returned together with a negative status code 
  tok_err, 

  tok_plus,
  tok_plus_equals,
  tok_minus,
  
  tok_colon,
  tok_double_colon,
  tok_semicolon,

  tok_arrow,

  tok_left_paren,
  tok_right_paren,
  tok_left_brace,
  tok_right_brace,
  tok_left_bracket,
  tok_right_bracket,

  tok_identifier,
  tok_integer,

  tok_return,
} token_kind_t;

typedef struct token_t {
  sv_t content;
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
int lexer_lex(lexer_t *lexer, token_t **tokens);

void lexer_recover(lexer_t *lexer);
int lexer_next_token(lexer_t *lexer);
void lexer_skip_whitespace(lexer_t *lexer);
location_t lexer_location(lexer_t *lexer, char *start);