#include "movegen.h"
#include "aurora.h"
#include "position.h"

#include <stdint.h>

int aurora_get_white_moves(Aurora *aurora, Move moves[100], uint64_t not_white_pieces, uint64_t black_pieces) {
  int nb_moves = 0;

  get_white_castling_moves(aurora, moves, &nb_moves);
 
  get_queens_moves(moves, &nb_moves, &aurora->white_queens, not_white_pieces, black_pieces, aurora->all_pieces);

  get_rooks_moves(moves, &nb_moves, &aurora->white_rooks, not_white_pieces, black_pieces, aurora->all_pieces);

  get_bishops_moves(moves, &nb_moves, &aurora->white_bishops, not_white_pieces, black_pieces, aurora->all_pieces);

  get_king_moves(moves, &nb_moves, &aurora->white_king, not_white_pieces, black_pieces);

  get_knights_moves(moves, &nb_moves, &aurora->white_knights, not_white_pieces, black_pieces); 

  get_white_pawns_moves(aurora, moves, &nb_moves, black_pieces);

  return nb_moves;
}

int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces) {
  int nb_moves = 0;

  get_black_castling_moves(aurora, moves, &nb_moves);

  get_queens_moves(moves, &nb_moves, &aurora->black_queens, not_black_pieces, white_pieces, aurora->all_pieces);

  get_rooks_moves(moves, &nb_moves, &aurora->black_rooks, not_black_pieces, white_pieces, aurora->all_pieces);

  get_bishops_moves(moves, &nb_moves, &aurora->black_bishops, not_black_pieces, white_pieces, aurora->all_pieces);

  get_king_moves(moves, &nb_moves, &aurora->black_king, not_black_pieces, white_pieces);

  get_knights_moves(moves, &nb_moves, &aurora->black_knights, not_black_pieces, white_pieces); 

  get_black_pawns_moves(aurora, moves, &nb_moves, white_pieces);

  return nb_moves;
}

int aurora_get_moves(Aurora *aurora, Move moves[100]) {
  int nb_moves = 0;

  aurora_analyse(aurora);

  if (aurora->turn) {
    uint64_t not_white_pieces = ~(aurora->all_white_pieces | aurora->black_king);
    uint64_t black_pieces = aurora->all_black_pieces ^ aurora->black_king;

    nb_moves = aurora_get_white_moves(aurora, moves, not_white_pieces, black_pieces);
  } else {
    uint64_t not_black_pieces = ~(aurora->all_black_pieces | aurora->white_king);
    uint64_t white_pieces = aurora->all_white_pieces ^ aurora->white_king;

    nb_moves = aurora_get_black_moves(aurora, moves, not_black_pieces, white_pieces);
  }

  return nb_moves;
}

void get_queens_moves(Move moves[100], int *nb_moves, uint64_t *this_side_queens, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied) {
  ////////////////////////////////////////////////////////// QUEENS MOVES
  uint64_t queens = *this_side_queens;
  uint64_t piece = queens & ~(queens - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t queens_attacks =
        (hyperbola_quintessence(piece, occupied,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, occupied,
                                diag_backslash[diag_index[i][1]]) |
         hyperbola_quintessence(piece, occupied, rank_mask(i)) |
         hyperbola_quintessence(piece, occupied, file_mask(i))) &
        not_my_pieces;

    uint64_t move = queens_attacks & ~(queens_attacks - 1);
    while (move) {
      Move queen_move = {this_side_queens,
                         piece | move,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL};

      if (move & opp_pieces)
        queen_move.capture = move;

      moves[(*nb_moves)++] = queen_move;

      // LOOP THROUGH THE QUEEN MOVES
      queens_attacks ^= move;
      move = queens_attacks & ~(queens_attacks - 1);
    }

    queens ^= piece;
    piece = queens & ~(queens - 1);
  }
}

void get_rooks_moves(Move moves[100], int *nb_moves, uint64_t *this_side_rooks, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied) {
  ////////////////////////////////////////////////////////// ROOKS MOVES
  uint64_t rooks = *this_side_rooks;
  uint64_t piece = rooks & ~(rooks - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t rooks_attacks =
        (hyperbola_quintessence(piece, occupied, rank_mask(i)) |
         hyperbola_quintessence(piece, occupied, file_mask(i))) &
        not_my_pieces;

    uint64_t move = rooks_attacks & ~(rooks_attacks - 1);
    while (move) {
      Move rook_move = {this_side_rooks,
                        piece | move,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL};

      if (move & opp_pieces)
        rook_move.capture = move;

      moves[(*nb_moves)++] = rook_move;

      // LOOP THROUGH THE ROOKS MOVES
      rooks_attacks ^= move;
      move = rooks_attacks & ~(rooks_attacks - 1);
    }

    rooks ^= piece;
    piece = rooks & ~(rooks - 1);
  }
}

void get_bishops_moves(Move moves[100], int *nb_moves, uint64_t *this_side_bishops, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied) {
  ////////////////////////////////////////////////////////// WHITE BISHOPS MOVES
  uint64_t bishops = *this_side_bishops;
  uint64_t piece = bishops & ~(bishops - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t bishops_attacks =
        (hyperbola_quintessence(piece, occupied,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, occupied,
                                diag_backslash[diag_index[i][1]])) &
        not_my_pieces;

    uint64_t move = bishops_attacks & ~(bishops_attacks - 1);
    while (move) {
      Move bishop_move = {this_side_bishops,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & opp_pieces)
        bishop_move.capture = move;

      moves[(*nb_moves)++] = bishop_move;

      // LOOP THROUGH THE BISHOPS MOVES
      bishops_attacks ^= move;
      move = bishops_attacks & ~(bishops_attacks - 1);
    } bishops ^= piece;
    piece = bishops & ~(bishops - 1);
  }
}

void get_knights_moves(Move moves[100], int *nb_moves, uint64_t *this_side_knights, uint64_t not_my_pieces, uint64_t opp_pieces) {
  ///////////////////////////////////////////////////////// WHITE KNIGHTS MOVES
  uint64_t knights = *this_side_knights;
  uint64_t piece = knights & ~(knights - 1);
  uint64_t knights_moves;

  while (piece) {
    int i = __builtin_ctzll(piece);

    if (i > 18)
      knights_moves = KNIGHT_SPAN << (i - 18) & not_my_pieces;
    else
      knights_moves = KNIGHT_SPAN >> (18 - i) & not_my_pieces;

    if (piece & FILE_AB)
      knights_moves &= ~FILE_GH;
    else if (piece & FILE_GH)
      knights_moves &= ~FILE_AB;

    uint64_t move = knights_moves & ~(knights_moves - 1);
    while (move) {
      Move knight_move = {this_side_knights,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & opp_pieces)
        knight_move.capture = move;

      moves[(*nb_moves)++] = knight_move;

      // LOOP THROUGH THE KNIGHTS MOVES
      knights_moves ^= move;
      move = knights_moves & ~(knights_moves - 1);
    }

    // LOOP THROUGH KNIGHTS
    knights ^= piece;
    piece = knights & ~(knights - 1);
  }
}

void get_king_moves(Move moves[100], int *nb_moves, uint64_t *this_side_king, uint64_t not_my_pieces, uint64_t opp_pieces) {
  ////////////////////////////////////////////////////////// KING MOVES
  uint64_t piece = *this_side_king;
  int i = __builtin_ctzll(piece);
  uint64_t king_moves;

  if (i > 9)
    king_moves = KING_SPAN << (i - 9) & not_my_pieces;
  else
    king_moves = KING_SPAN >> (9 - i) & not_my_pieces;

  if (piece & FILE_A)
    king_moves &= ~FILE_H;
  else if (piece & FILE_H)
    king_moves &= ~FILE_A;

  uint64_t move = king_moves & ~(king_moves - 1);
  while (move) {   
    Move king_move = {this_side_king, piece | move, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL};

    if (move & opp_pieces)
      king_move.capture = move;

    moves[(*nb_moves)++] = king_move;

    // LOOP THROUGH THE KING MOVES
    king_moves ^= move;
    move = king_moves & ~(king_moves - 1);
  }
}

void get_white_castling_moves(Aurora *aurora, Move moves[100], int *nb_moves) {
  ////////////////////////////////////////////////////////// WHITE CASTLING
  // KING SIDE CASTLE
  if (aurora->castling_right[0] && 0x20ULL & aurora->empty && 0x40ULL & aurora->empty) {
    moves[(*nb_moves)++] =
        (Move){&aurora->white_king, 0x50ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0xA0ULL, &aurora->white_rooks};
  }

  // QUEEN SIDE CASTLE
  if (aurora->castling_right[1] && 0x2ULL & aurora->empty && 0x4ULL & aurora->empty &&
      0x8ULL & aurora->empty) {
    moves[(*nb_moves)++] =
        (Move){&aurora->white_king, 0x14ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0x9ULL, &aurora->white_rooks};
  }
}

void get_black_castling_moves(Aurora *aurora, Move moves[100], int *nb_moves) {
  ////////////////////////////////////////////////////////// BLACK CASTLING
  // KING SIDE CASTLE
  if (aurora->castling_right[2] && 0x2000000000000000ULL & aurora->empty &&
      0x4000000000000000ULL & aurora->empty) {
    moves[(*nb_moves)++] = (Move){
        &aurora->black_king,   0x5000000000000000ULL, 0ULL, 0ULL, 0ULL, 0ULL,
        0xA000000000000000ULL, &aurora->black_rooks};
  }

  // QUEEN SIDE CASTLE
  if (aurora->castling_right[3] && 0x200000000000000ULL & aurora->empty &&
      0x400000000000000ULL & aurora->empty && 0x800000000000000ULL & aurora->empty) {
    moves[(*nb_moves)++] = (Move){
        &aurora->black_king,  0x1400000000000000ULL, 0ULL, 0ULL, 0ULL, 0ULL,
        0x900000000000000ULL, &aurora->black_rooks};
  }
}


void get_white_pawns_moves(Aurora *aurora, Move moves[100], int *nb_moves, uint64_t black_pieces) {
  /////////////////////////////////////////////////////////// WHITE PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->white_pawns << 8) & aurora->empty & ~RANK_8;
  uint64_t piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    forward_moves ^= piece;
    piece = forward_moves & ~(forward_moves - 1);
  }

  // PAWN MOVE FORWARD 2
  uint64_t double_forward_moves =
      (aurora->white_pawns << 16) & RANK_4 & aurora->empty & (aurora->empty << 8);
  piece = double_forward_moves & ~(double_forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 16),
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    double_forward_moves ^= piece;
    piece = double_forward_moves & ~(double_forward_moves - 1);
  }

  // CAPTURE RIGHT
  uint64_t capture_right_moves =
      (aurora->white_pawns << 9) & black_pieces & ~FILE_A & ~RANK_8;
  piece = capture_right_moves & ~(capture_right_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    capture_right_moves ^= piece;
    piece = capture_right_moves & ~(capture_right_moves - 1);
  }

  // CAPTURE LEFT
  uint64_t capture_left_moves =
      (aurora->white_pawns << 7) & black_pieces & ~FILE_H & ~RANK_8;
  piece = capture_left_moves & ~(capture_left_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    capture_left_moves ^= piece;
    piece = capture_left_moves & ~(capture_left_moves - 1);
  }

  // EN PASSANT CAPTURE RIGHT
  uint64_t en_passant_capture_right =
      (aurora->white_pawns << 1) & aurora->black_pawns & RANK_5 & ~FILE_A;
  piece = en_passant_capture_right & ~(en_passant_capture_right - 1);

  while (piece) {
    // CHECK IF CAN EN PASSANT
    if ((piece << 8) & aurora->en_passant) {
      moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                                 (piece >> 1) | aurora->en_passant,
                                 piece,
                                 &aurora->black_pawns,
                                 0ULL,
                                 0ULL,
                                 0ULL,
                                 0ULL};
    }

    // LOOP
    en_passant_capture_right ^= piece;
    piece = en_passant_capture_right & ~(en_passant_capture_right - 1);
  }

  // EN PASSANT CAPTURE LEFT
  uint64_t en_passant_capture_left =
      (aurora->white_pawns >> 1) & aurora->black_pawns & RANK_5 & ~FILE_H;
  piece = en_passant_capture_left & ~(en_passant_capture_left - 1);

  while (piece) {
    // CHECK IF CAN EN PASSANT
    if ((piece << 8) & aurora->en_passant) {
      moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                                 (piece << 1) | aurora->en_passant,
                                 piece,
                                 &aurora->black_pawns,
                                 0ULL,
                                 0ULL,
                                 0ULL,
                                 0ULL};
    }

    // LOOP
    en_passant_capture_left ^= piece;
    piece = en_passant_capture_left & ~(en_passant_capture_left - 1);
  }

  // PROMOTION FORWARD
  uint64_t promotion_forward = (aurora->white_pawns << 8) & RANK_8 & aurora->empty;
  piece = promotion_forward & ~(promotion_forward - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_queens,
                               0ULL,
                               0ULL};

    promotion_forward ^= piece;
    piece = promotion_forward & ~(promotion_forward - 1);
  }

  // PROMOTION CAPTURE RIGHT
  uint64_t promotion_capture_right =
      (aurora->white_pawns << 9) & black_pieces & RANK_8 & ~FILE_A;
  piece = promotion_capture_right & ~(promotion_capture_right - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_queens,
                               0ULL,
                               0ULL};

    promotion_capture_right ^= piece;
    piece = promotion_capture_right & ~(promotion_capture_right - 1);
  }

  // PROMOTION CAPTURE LEFT
  uint64_t promotion_capture_left =
      (aurora->white_pawns << 7) & black_pieces & RANK_8 & ~FILE_H;
  piece = promotion_capture_left & ~(promotion_capture_left - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_queens,
                               0ULL,
                               0ULL};

    promotion_capture_left ^= piece;
    piece = promotion_capture_left & ~(promotion_capture_left - 1);
  }
}

void get_black_pawns_moves(Aurora *aurora, Move moves[100], int *nb_moves, uint64_t white_pieces) {
  //////////////////////////////////////////////////////////// BLACK PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->black_pawns >> 8) & aurora->empty & ~RANK_1;
  uint64_t piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    forward_moves ^= piece;
    piece = forward_moves & ~(forward_moves - 1);
  }

  // PAWN MOVE FORWARD 2
  uint64_t double_forward_moves =
      (aurora->black_pawns >> 16) & RANK_5 & aurora->empty & (aurora->empty >> 8);
  piece = double_forward_moves & ~(double_forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 16),
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    double_forward_moves ^= piece;
    piece = double_forward_moves & ~(double_forward_moves - 1);
  }

  // CAPTURE RIGHT
  uint64_t capture_right_moves =
      (aurora->black_pawns >> 9) & white_pieces & ~FILE_H & ~RANK_1;
  piece = capture_right_moves & ~(capture_right_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               piece,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    capture_right_moves ^= piece;
    piece = capture_right_moves & ~(capture_right_moves - 1);
  }

  // CAPTURE LEFT
  uint64_t capture_left_moves =
      (aurora->black_pawns >> 7) & white_pieces & ~FILE_A & ~RANK_1;
  piece = capture_left_moves & ~(capture_left_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               piece,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL,
                               0ULL};

    // LOOP
    capture_left_moves ^= piece;
    piece = capture_left_moves & ~(capture_left_moves - 1);
  }

  // EN PASSANT CAPTURE RIGHT
  uint64_t en_passant_capture_right =
      (aurora->black_pawns >> 1) & aurora->white_pawns & RANK_4 & ~FILE_H;
  piece = en_passant_capture_right & ~(en_passant_capture_right - 1);

  while (piece) {
    // CHECK IF CAN EN PASSANT
    if ((piece >> 8) & aurora->en_passant) {
      moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                                 (piece << 1) | aurora->en_passant,
                                 piece,
                                 &aurora->white_pawns,
                                 0ULL,
                                 0ULL,
                                 0ULL,
                                 0ULL};
    }

    // LOOP
    en_passant_capture_right ^= piece;
    piece = en_passant_capture_right & ~(en_passant_capture_right - 1);
  }

  // EN PASSANT CAPTURE LEFT
  uint64_t en_passant_capture_left =
      (aurora->black_pawns << 1) & aurora->white_pawns & RANK_4 & ~FILE_A;
  piece = en_passant_capture_left & ~(en_passant_capture_left - 1);

  while (piece) {
    // CHECK IF CAN EN PASSANT
    if ((piece >> 8) & aurora->en_passant) {
      moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                                 (piece >> 1) | aurora->en_passant,
                                 piece,
                                 &aurora->white_pawns,
                                 0ULL,
                                 0ULL,
                                 0ULL,
                                 0ULL};
    }

    // LOOP
    en_passant_capture_left ^= piece;
    piece = en_passant_capture_left & ~(en_passant_capture_left - 1);
  }

  // PROMOTION FORWARD
  uint64_t promotion_forward = (aurora->black_pawns >> 8) & RANK_1 & aurora->empty;
  piece = promotion_forward & ~(promotion_forward - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_queens,
                               0ULL,
                               0ULL};

    promotion_forward ^= piece;
    piece = promotion_forward & ~(promotion_forward - 1);
  }

  // PROMOTION CAPTURE RIGHT
  uint64_t promotion_capture_right =
      (aurora->black_pawns >> 9) & white_pieces & RANK_1 & ~FILE_H;
  piece = promotion_capture_right & ~(promotion_capture_right - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_queens,
                               0ULL,
                               0ULL};

    promotion_capture_right ^= piece;
    piece = promotion_capture_right & ~(promotion_capture_right - 1);
  }

  // PROMOTION CAPTURE LEFT
  uint64_t promotion_capture_left =
      (aurora->black_pawns >> 7) & white_pieces & RANK_1 & ~FILE_A;
  piece = promotion_capture_left & ~(promotion_capture_left - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[(*nb_moves)++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_queens,
                               0ULL,
                               0ULL};

    promotion_capture_left ^= piece;
    piece = promotion_capture_left & ~(promotion_capture_left - 1);
  }

}

uint64_t hyperbola_quintessence(uint64_t piece, uint64_t occupied,
                                uint64_t mask) {
  return (((mask & occupied) - 2 * piece) ^
          reverse(reverse(mask & occupied) - reverse(piece) * 2)) &
         mask;
};

uint64_t reverse(uint64_t b) {
  b = (b & 0x5555555555555555) << 1 | ((b >> 1) & 0x5555555555555555);
  b = (b & 0x3333333333333333) << 2 | ((b >> 2) & 0x3333333333333333);
  b = (b & 0x0f0f0f0f0f0f0f0f) << 4 | ((b >> 4) & 0x0f0f0f0f0f0f0f0f);
  b = (b & 0x00ff00ff00ff00ff) << 8 | ((b >> 8) & 0x00ff00ff00ff00ff);

  return (b << 48) | ((b & 0xffff0000) << 16) | ((b >> 16) & 0xffff0000) |
         (b >> 48);
}
