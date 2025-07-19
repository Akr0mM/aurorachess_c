#include "test.h"
#include "../aurora.h"
#include "../board.h"
#include "../movegen.h"
#include "../position.h"

#include <stdio.h>

int main(void) {
  Aurora aurora;
  char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  aurora_init(&aurora, fen);
 
  printf("Test 1 : Bitboard Correspondence -> %s\n", bitboard_correspondence(&aurora) ? "Passed ✅" : "Failed ❌");
  
  return 0;
}

int bitboard_correspondence(Aurora *aurora) {
  char list_of_moves[62][5] = {
      "e2e4", "e7e5", "g1f3", "b8c6", "b1c3", "d7d6", "d2d4", "e5d4", "f3d4",
      "c6d4", "d1d4", "f8e7", "c1e3", "g8f6", "e1c1", "b7b6", "a2a3", "c8b7",
      "f2f4", "e8g8", "h2h3", "h7h6", "g2g4", "f6g4", "h3g4", "e7f6", "d4b4",
      "f6c3", "b4c3", "d8e7", "f1g2", "b7e4", "g2e4", "e7e4", "c3c7", "e4e3",
      "c1b1", "e3f4", "c7d6", "f4d6", "d1d6", "h6h5", "g4g5", "f7f5", "g5f6",
      "g7f6", "d6f6", "f8f6", "h1g1", "f6g6", "g1g6", "g8h7", "g6h6", "h7h6",
      "b1c1", "h5h4", "c1d2", "h4h3", "d2e1", "h3h2", "e1f2", "h2h1q"};

  Move all_moves[100];

  for (int i = 0; i < 62; i++) {
    aurora_get_moves(aurora, all_moves);
    aurora_make_move(aurora, get_move_from_sq(all_moves, list_of_moves[i]));
    draw_board_bitboard(aurora->bitboards[ALL]);
  }

  aurora_analyse(aurora);

  if (
     	aurora->bitboards[WP] == 0x10600ULL &&
     	aurora->bitboards[WN] == 0ULL &&
     	aurora->bitboards[WB] == 0ULL &&
     	aurora->bitboards[WR] == 0ULL &&
     	aurora->bitboards[WQ] == 0ULL &&
     	aurora->bitboards[WK] == 0x2000ULL &&
     	aurora->bitboards[BP] == 0x1020000000000ULL &&
     	aurora->bitboards[BN] == 0ULL &&
     	aurora->bitboards[BB] == 0ULL &&
     	aurora->bitboards[BR] == 0x100000000000000ULL &&
     	aurora->bitboards[BQ] == 0x80ULL &&
     	aurora->bitboards[BK] == 0x800000000000ULL &&
     	aurora->bitboards[WHITE] == 0x12600ULL &&
     	aurora->bitboards[BLACK] == 0x101820000000080ULL && 
     	aurora->bitboards[ALL] == 0x101820000012680ULL 
     ) return 1;
  return 0;
}
