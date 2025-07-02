#include "board.h"
#include "aurora.h"

#include <stdint.h>
#include <stdio.h>

void draw_all_moves(Move moves[100], int nb_moves) {
  int nb_captures = 0;
  for (int i = 0; i < nb_moves; i++) {
    draw_board_bitboard(moves[i].move);
    if (moves[i].capture)
      nb_captures++;
  }

  printf("Total number of moves found : %d\n", nb_moves);
  printf("Total number of captures found : %d\n", nb_captures);
}

void draw_board_bitboard(uint64_t bitboard) {
  puts("   -----------------");

  for (int rank = 7; rank >= 0; rank--) {
    char line[22];

    for (int file = 0; file < 8; file++) {
      uint64_t piece = 1ULL << (rank * 8 + file);
      line[2 * file + 4] = (bitboard & piece) ? 'X' : '0';
      line[2 * file + 5] = ' ';
    }

    line[0] = '0' + (rank + 1);
    line[1] = ' ';
    line[2] = '|';
    line[3] = ' ';
    line[19] = ' ';
    line[20] = '|';
    line[21] = '\0';
    puts(line);
  }

  puts("   -----------------");
  puts("    a b c d e f g h");
}
