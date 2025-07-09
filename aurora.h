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
  uint64_t castle;
  uint64_t *rook;
} Move;

// ENGINE STRUCT
typedef struct Aurora {
  int turn;
  int castling_right[4];
  uint64_t en_passant;

  uint64_t all_pieces;
  uint64_t empty;
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

static const uint64_t diag_slash[15] = {
    0x0000000000000001ULL, /* sum=0  : a1 */
    0x0000000000000102ULL, /* sum=1  : a2,b1 */
    0x0000000000010204ULL, /* sum=2  : a3,b2,c1 */
    0x0000000001020408ULL, /* sum=3  : a4,b3,c2,d1 */
    0x0000000102040810ULL, /* sum=4  : a5,b4,c3,d2,e1 */
    0x0000010204081020ULL, /* sum=5  : a6,b5,c4,d3,e2,f1 */
    0x0001020408102040ULL, /* sum=6  : a7,b6,c5,d4,e3,f2,g1 */
    0x0102040810204080ULL, /* sum=7  : a8,b7,c6,d5,e4,f3,g2,h1 */
    0x0204081020408000ULL, /* sum=8  : b8,c7,d6,e5,f4,g3,h2 */
    0x0408102040800000ULL, /* sum=9  : c8,d7,e6,f5,g4,h3 */
    0x0810204080000000ULL, /* sum=10 : d8,e7,f6,g5,h4 */
    0x1020408000000000ULL, /* sum=11 : e8,f7,g6,h5 */
    0x2040800000000000ULL, /* sum=12 : f8,g7,h6 */
    0x4080000000000000ULL, /* sum=13 : g8,h7 */
    0x8000000000000000ULL  /* sum=14 : h8 */
};

/* Diagonales “\” (diff = −7..+7) */
static const uint64_t diag_backslash[15] = {
    0x0100000000000000ULL, /* diff=−7: a8 */
    0x0201000000000000ULL, /* diff=−6: a7,b8 */
    0x0402010000000000ULL, /* diff=−5: a6,b7,c8 */
    0x0804020100000000ULL, /* diff=−4: a5,b6,c7,d8 */
    0x1008040201000000ULL, /* diff=−3: a4,b5,c6,d7,e8 */
    0x2010080402010000ULL, /* diff=−2: a3,b4,c5,d6,e7,f8 */
    0x4020100804020100ULL, /* diff=−1: a2,b3,c4,d5,e6,f7,g8 */
    0x8040201008040201ULL, /* diff=0 : a1,b2,c3,d4,e5,f6,g7,h8 */
    0x0080402010080402ULL, /* diff=+1: b1,c2,d3,e4,f5,g6,h7 */
    0x0000804020100804ULL, /* diff=+2: c1,d2,e3,f4,g5,h6 */
    0x0000008040201008ULL, /* diff=+3: d1,e2,f3,g4,h5 */
    0x0000000080402010ULL, /* diff=+4: e1,f2,g3,h4 */
    0x0000000000804020ULL, /* diff=+5: f1,g2,h3 */
    0x0000000000008040ULL, /* diff=+6: g1,h2 */
    0x0000000000000080ULL  /* diff=+7: h1 */
};

static const uint8_t diag_index[64][2] = {
    {0, 7},  // a1
    {1, 8},  // b1
    {2, 9},  // c1
    {3, 10}, // d1
    {4, 11}, // e1
    {5, 12}, // f1
    {6, 13}, // g1
    {7, 14}, // h1

    {1, 6},  // a2
    {2, 7},  // b2
    {3, 8},  // c2
    {4, 9},  // d2
    {5, 10}, // e2
    {6, 11}, // f2
    {7, 12}, // g2
    {8, 13}, // h2

    {2, 5},  // a3
    {3, 6},  // b3
    {4, 7},  // c3   ← diag_index[18][1] = 7
    {5, 8},  // d3
    {6, 9},  // e3
    {7, 10}, // f3
    {8, 11}, // g3
    {9, 12}, // h3

    {3, 4},   // a4
    {4, 5},   // b4
    {5, 6},   // c4
    {6, 7},   // d4
    {7, 8},   // e4
    {8, 9},   // f4
    {9, 10},  // g4
    {10, 11}, // h4

    {4, 3},   // a5
    {5, 4},   // b5
    {6, 5},   // c5
    {7, 6},   // d5
    {8, 7},   // e5
    {9, 8},   // f5
    {10, 9},  // g5
    {11, 10}, // h5

    {5, 2},  // a6
    {6, 3},  // b6
    {7, 4},  // c6
    {8, 5},  // d6
    {9, 6},  // e6
    {10, 7}, // f6
    {11, 8}, // g6
    {12, 9}, // h6

    {6, 1},  // a7
    {7, 2},  // b7
    {8, 3},  // c7
    {9, 4},  // d7
    {10, 5}, // e7
    {11, 6}, // f7
    {12, 7}, // g7
    {13, 8}, // h7

    {7, 0},  // a8
    {8, 1},  // b8
    {9, 2},  // c8
    {10, 3}, // d8
    {11, 4}, // e8
    {12, 5}, // f8
    {13, 6}, // g8
    {14, 7}  // h8;
};
