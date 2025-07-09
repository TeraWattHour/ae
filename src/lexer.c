#include "lexer.h"
#include "dynamic_array.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#define tok_single(k) return lexer_set(k, 1)
#define tok_double(p, k) if (strncmp(lexer->c, p, 2) == 0) return lexer_set(k, 2)
#define lexer_set(k, n) ({ lexer->c += (n); lexer->token = (token_t) { .content = sv(lexer->s, lexer->c), .kind = (k) }; k; }) 
#define is_identifier_char(c) ( isascii(c) && isalnum(c) || c == '_' )
#define report_err(where, fmt, ...) ({ location_t loc = lexer_location(lexer, where); fprintf(stderr, "%s:%zu:%zu: error: " fmt "\n", lexer->file, loc.line, loc.column, ##__VA_ARGS__); -1; })

#define map(k) { .kind = (k), .literal = (#k)+4 }
typedef struct identifier_mapping_t {
  token_kind_t kind;
  char *literal;
} identifier_mapping_t;

static const identifier_mapping_t identifier_mappings[] = {
  map(tok_return),
};
#undef map

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

int lexer_lex(lexer_t *lexer, token_t **tokens) {
  int ret;
  bool failed = false;

  do {
    ret = lexer_next_token(lexer);
    if (ret < 0) {
      lexer_recover(lexer);
      failed = true;
      if (lexer->token.kind == tok_err) continue;
    }

    if (lexer->token.kind > tok_err) da_append(*tokens, lexer->token);
  } while (lexer->token.kind);

  return failed;
}

int lexer_next_token(lexer_t *lexer) {
  assert(lexer && lexer->c);
  
  lexer_skip_whitespace(lexer);
  
  lexer->s = lexer->c;
  if (!*lexer->c) return lexer_set(tok_eof, 0);

  switch (*lexer->c) {
    case '+': tok_double("+=", tok_plus_equals); tok_single(tok_plus);
    case '-': tok_double("->", tok_arrow); tok_single(tok_minus);
    case ':': tok_double("::", tok_double_colon); tok_single(tok_colon);
    case ';': tok_single(tok_semicolon);

    case '{': tok_single(tok_left_brace);
    case '}': tok_single(tok_right_brace);
    case '(': tok_single(tok_left_paren);
    case ')': tok_single(tok_right_paren);
    case '[': tok_single(tok_left_bracket);
    case ']': tok_single(tok_right_bracket);

    case 'A'...'Z':
    case 'a'...'z':
    case '_': {
      while (is_identifier_char(*lexer->c)) lexer->c++;

      token_kind_t kind = tok_identifier;
      for (size_t i = 0; i < sizeof(identifier_mappings) / sizeof(identifier_mappings[0]); i++) {
        identifier_mapping_t mapping = identifier_mappings[i];
        if (strncmp(mapping.literal, lexer->s, lexer->c - lexer->s) == 0) {
          kind = mapping.kind;
          break;
        }
      } 

      lexer_set(kind, 0);
      if (!isascii(*lexer->c)) return report_err(lexer->c, "unexpected character in identifier");

      return kind;
    }

    case '1'...'9': {
      while (isdigit(*++lexer->c)) {}
      lexer_set(tok_integer, 0);

      if (!isascii(*lexer->c) || isalnum(*lexer->c)) return report_err(lexer->c, "unexpected character just after an integer literal");
      
      return tok_integer;
    }
  }

  lexer_set(tok_err, 0);
  return report_err(lexer->c, "unexpected character");
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

void lexer_recover(lexer_t *lexer) {
  while (*lexer->c && !isspace(*lexer->c) && *lexer->c != '}' && *lexer->c != ')' && *lexer->c != ']') lexer->c++;
}