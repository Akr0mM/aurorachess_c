#include "aurora.h"
#include "board.h"
#include "movegen.h"
#include "position.h"

int main(void) {
  char *fen =
      "5p1p/2r3P1/1b6/2Ppp3/npKpnRr1/1NQPN1R1/PP3PPP/q1BpBbk1 w HAha - 0 1";
  // char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Aurora aurora;
  aurora_init(&aurora, fen);

  Move all_moves[100];
  int nb_moves = aurora_get_moves(&aurora, all_moves);
  // draw_all_moves(all_moves, nb_moves);

  make_move(&aurora, all_moves[14]);

  return 0;
}
