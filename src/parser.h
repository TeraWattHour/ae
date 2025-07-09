#pragma once

#include "./lexer.h"

typedef struct parser_t {
  lexer_t *lexer;
  token_t current;
  token_t next;
} parser_t;

typedef enum node_kind_t {
  node_identifier,
} node_kind_t;

typedef enum statement_kind_t {
  stmt_err,
  stmt_const_bind,
} statement_kind_t;

typedef struct node_t {
  node_kind_t kind;

  union {
    sv_t identifier;
  } as;
} node_t;

typedef struct statement_t {
  statement_kind_t kind;

  union {
    struct {
      node_t *lhs;
      node_t *rhs;
    } const_bind;
  } as;
} statement_t;

parser_t parser_init(lexer_t *lexer);
token_t parser_next_token(parser_t *parser);

node_t *parser_next_expression(parser_t *parser);
node_t *parser_next_lvalue(parser_t *parser);

statement_t *parser_next_statement(parser_t *parser);


node_t *new_identifier(token_t token);
statement_t *new_bind(node_t *rhs, node_t *lhs);

