#pragma once

#include <stdint.h>

// MOVE STRUCT
typedef struct Move {
  uint64_t *piece;
  uint64_t move;
  uint64_t capture;
  uint64_t *en_passant_piece_captured;
  uint64_t promotion;
  uint64_t *promotion_piece;
} Move;

// ENGINE STRUCT
typedef struct Aurora {
  int turn;
  uint64_t en_passant;

  uint64_t all_pieces;
  uint64_t all_white_pieces;
  uint64_t all_black_pieces;

  uint64_t white_pawns;
  uint64_t white_knights;
  uint64_t white_bishops;
  uint64_t white_rooks;
  uint64_t white_queens;
  uint64_t white_king;

  uint64_t black_pawns;
  uint64_t black_knights;
  uint64_t black_bishops;
  uint64_t black_rooks;
  uint64_t black_queens;
  uint64_t black_king;

  uint64_t *board_bitboard[64];
} Aurora;

//// CONSTANT VARIABLE
// FILE
static const uint64_t FILE_A = 0x0101010101010101ULL;
static const uint64_t FILE_B = FILE_A << 1;
static const uint64_t FILE_C = FILE_A << 2;
static const uint64_t FILE_D = FILE_A << 3;
static const uint64_t FILE_E = FILE_A << 4;
static const uint64_t FILE_F = FILE_A << 5;
static const uint64_t FILE_G = FILE_A << 6;
static const uint64_t FILE_H = FILE_A << 7;
static const uint64_t FILE_AB = FILE_A | FILE_B;
static const uint64_t FILE_GH = FILE_G | FILE_H;

// RANK
static const uint64_t RANK_1 = 0xFFULL;
static const uint64_t RANK_2 = RANK_1 << (8 * 1);
static const uint64_t RANK_3 = RANK_1 << (8 * 2);
static const uint64_t RANK_4 = RANK_1 << (8 * 3);
static const uint64_t RANK_5 = RANK_1 << (8 * 4);
static const uint64_t RANK_6 = RANK_1 << (8 * 5);
static const uint64_t RANK_7 = RANK_1 << (8 * 6);
static const uint64_t RANK_8 = RANK_1 << (8 * 7);

// USEFUL MASK
static const uint64_t CENTER = 0x1818000000ULL;
static const uint64_t EXTENDED_CENTER = 0x3C3C3C3C0000ULL;
static const uint64_t KING_SIDE = FILE_E | FILE_F | FILE_G | FILE_H;
static const uint64_t QUEEN_SIDE = FILE_A | FILE_B | FILE_C | FILE_D;
static const uint64_t KNIGHT_SPAN = 0xA1100110AULL;
static const uint64_t KING_SPAN = 0x70507ULL;
static const uint64_t DOUBLE_FORWARD_PAWN_SPAN = 0x10001ULL;

// FUNCTIONS
void aurora_init(Aurora *aurora, char *fen);
uint64_t square_to_bitboard(char square[2]);
