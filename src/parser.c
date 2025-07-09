#include "./parser.h"

#include <stdio.h>
#include <stdlib.h>

#define new(t) ((t*)malloc(sizeof(t)))

parser_t parser_init(lexer_t *lexer) {
  parser_t parser = {
    .lexer = lexer,
    .current = {0},
    .next = {0},
  };

  parser_next_token(&parser);

  return parser;
}

token_t parser_next_token(parser_t *parser) {
  parser->current = parser->next;
  lexer_next_token(parser->lexer);
  parser->next = parser->lexer->token;
  return parser->current;
}

statement_t *parser_next_statement(parser_t *parser) {
  switch (parser_next_token(parser).kind) {
    case tok_identifier: {
      node_t *lhs = new_identifier(parser->current);
      
      if (parser->next.kind == tok_double_colon) {
        parser_next_token(parser);

        node_t *rhs = parser_next_expression(parser);

        return new_bind(lhs, rhs);
      }
    }
  }
}

node_t *parser_next_expression(parser_t *parser) {
  switch (parser_next_token(parser).kind) {
    case tok_identifier: return new_identifier(parser->current);
  }
}

node_t *parser_next_lvalue(parser_t *parser) {
  switch (parser_next_token(parser).kind) {
    case tok_identifier: return new_identifier(parser->current);
  }
}

node_t *new_identifier(token_t token) {
  node_t *i = new(node_t);
  (*i) = (node_t){ .kind = node_identifier, .as.identifier = token.content };
  return i;
}

statement_t *new_bind(node_t *rhs, node_t *lhs) {
  statement_t *s = new(statement_t);
  (*s) = (statement_t) { .kind = stmt_const_bind, .as.const_bind = { .lhs = lhs, .rhs = rhs } };
  return s;
}

