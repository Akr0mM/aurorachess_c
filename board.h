#pragma once

#include "aurora.h"

#include <stdint.h>

void draw_board_fen(char *fen);
void draw_board_bitboard(uint64_t bitboard);
void draw_all_moves(Move moves[100], int nb_moves);
