#include "aurora.h"
#include "position.h"

#include <stdint.h>

void aurora_init(Aurora *aurora, char *fen) {
  // INIT ENGINE
  aurora_load_fen(aurora, fen);
}

uint64_t square_to_bitboard(char square[2]) {
  uint64_t bb = 1ULL;

  bb <<= ((square[1] - '1') * 8 + square[0] - 'a');

  return bb;
}
