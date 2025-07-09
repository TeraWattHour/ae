#include "lexer.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define tok_single(k) return lexer_set(k, 1)
#define tok_double(p, k) if (strncmp(lexer->c, p, 2) == 0) return lexer_set(k, 2)
#define lexer_set(k, n) ({ lexer->c += (n); lexer->token = (token_t) { .start = lexer->s, .end = lexer->c, .kind = (k) }; k; }) 
#define is_identifier_char(c) ( isascii(c) && isalnum(c) || c == '_' )
#define report_err(where, fmt, ...) ({ location_t loc = lexer_location(lexer, where); fprintf(stderr, "%s:%zu:%zu: error: " fmt "\n", lexer->file, loc.line, loc.column, ##__VA_ARGS__); -1; })

lexer_t lexer_init(char *file, char *source) {
  return (lexer_t) {
    .content = source,
    .file = file,

    .line = 1,
    .column = 1,

    .c = source,
    .s = source,
    .token = {0}
  };
}

int lexer_next_token(lexer_t *lexer) {
  assert(lexer && lexer->c);
  
  lexer_skip_whitespace(lexer);
  
  lexer->s = lexer->c;
  if (!*lexer->c) return lexer_set(tok_eof, 0);

  switch (*lexer->c) {
    case '+': tok_double("+=", tok_plus_equals); tok_single(tok_plus);
    case '-': tok_single(tok_minus);

    case 'A'...'Z':
    case 'a'...'z':
    case '_': {
      while (is_identifier_char(*lexer->c)) lexer->c++;
      if (!isascii(*lexer->c)) return report_err(lexer->c, "unexpected character in identifier");
      return lexer_set(tok_identifier, 0);
    }

    case '1'...'9': {
      while (isdigit(*++lexer->c)) {}
      if (!isascii(*lexer->c) || isalnum(*lexer->c)) return report_err(lexer->c, "unexpected character just after an integer literal");
      return lexer_set(tok_integer, 0);
    }
  }

  return -1;
}

void lexer_skip_whitespace(lexer_t *lexer) {
  assert(lexer && lexer->c);

  while (isspace(*lexer->c)) lexer->c++;
}

location_t lexer_location(lexer_t *lexer, char *start) {
  size_t line = 1;
  size_t column = 1;
  char *c = lexer->content;
  while (c < start) {
    if (*c == '\n') { line++; column = 1; }
    else { column++; }

    c++;
  }
  return (location_t) { .column = column, .line = line, .file = lexer->file }; 
}