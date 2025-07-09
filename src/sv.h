#pragma once

#define sv(s, e) ((sv_t) { .start = (s), .end = (e) })
#define sv_print(s) (int)(s.end - s.start), s.start

typedef struct sv_t {
  const char *start;
  const char *end;
} sv_t;
