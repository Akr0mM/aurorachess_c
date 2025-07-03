#include "aurora.h"
#include "board.h"
#include "movegen.h"
#include "position.h"

int main(void) {
  char *fen = "r4rk1/p4pp1/1p1R4/6Pp/8/P7/1PP5/1K5R b - - 0 22";
  // char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Aurora aurora;
  aurora_init(&aurora, fen);

  Move all_moves[100];
  int nb_moves = aurora_get_moves(&aurora, all_moves);
  draw_all_moves(all_moves, nb_moves);
  aurora_make_move(&aurora, get_move_from_sq(all_moves, "f7f5"));
  // aurora_get_moves(&aurora, all_moves);
  // draw_board_bitboard(aurora.all_black_pieces);

  return 0;
}
