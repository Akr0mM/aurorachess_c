#include "movegen.h"
#include "aurora.h"
#include "position.h"
#include <stdint.h>

int aurora_get_white_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_white_pieces, uint64_t black_pieces,
                           uint64_t empty) {
  int nb_moves = 0;

  ////////////////////////////////////////////////////////// WHITE CASTLING
  // KING SIDE CASTLE
  if (aurora->castling_right[0] && 0x20ULL & empty && 0x40ULL & empty) {
    moves[nb_moves++] =
        (Move){&aurora->white_king, 0x50ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0xA0ULL,
               &aurora->white_rooks};
  }

  // QUEEN SIDE CASTLE
  if (aurora->castling_right[1] && 0x2ULL & empty && 0x4ULL & empty &&
      0x8ULL & empty) {
    moves[nb_moves++] =
        (Move){&aurora->white_king, 0x14ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0x9ULL,
               &aurora->white_rooks};
  }

  ////////////////////////////////////////////////////////// WHITE QUEENS MOVES
  uint64_t queens = aurora->white_queens;
  uint64_t piece = queens & ~(queens - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t queens_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_backslash[diag_index[i][1]]) |
         hyperbola_quintessence(piece, aurora->all_pieces, rank_mask(i)) |
         hyperbola_quintessence(piece, aurora->all_pieces, file_mask(i))) &
        not_white_pieces;

    uint64_t move = queens_attacks & ~(queens_attacks - 1);
    while (move) {
      Move queen_move = {&aurora->white_queens,
                         piece | move,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL};

      if (move & black_pieces)
        queen_move.capture = move;

      moves[nb_moves++] = queen_move;

      // LOOP THROUGH THE QUEEN MOVES
      queens_attacks ^= move;
      move = queens_attacks & ~(queens_attacks - 1);
    }

    queens ^= piece;
    piece = queens & ~(queens - 1);
  }

  ////////////////////////////////////////////////////////// WHITE ROOKS MOVES
  uint64_t rooks = aurora->white_rooks;
  piece = rooks & ~(rooks - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t rooks_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces, rank_mask(i)) |
         hyperbola_quintessence(piece, aurora->all_pieces, file_mask(i))) &
        not_white_pieces;

    uint64_t move = rooks_attacks & ~(rooks_attacks - 1);
    while (move) {
      Move rook_move = {&aurora->white_rooks,
                        piece | move,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL};

      if (move & black_pieces)
        rook_move.capture = move;

      moves[nb_moves++] = rook_move;

      // LOOP THROUGH THE ROOKS MOVES
      rooks_attacks ^= move;
      move = rooks_attacks & ~(rooks_attacks - 1);
    }

    rooks ^= piece;
    piece = rooks & ~(rooks - 1);
  }

  ////////////////////////////////////////////////////////// WHITE BISHOPS MOVES
  uint64_t bishops = aurora->white_bishops;
  piece = bishops & ~(bishops - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t bishops_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_backslash[diag_index[i][1]])) &
        not_white_pieces;

    uint64_t move = bishops_attacks & ~(bishops_attacks - 1);
    while (move) {
      Move bishop_move = {&aurora->white_bishops,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & black_pieces)
        bishop_move.capture = move;

      moves[nb_moves++] = bishop_move;

      // LOOP THROUGH THE BISHOPS MOVES
      bishops_attacks ^= move;
      move = bishops_attacks & ~(bishops_attacks - 1);
    }

    bishops ^= piece;
    piece = bishops & ~(bishops - 1);
  }

  ////////////////////////////////////////////////////////// WHITE KING MOVES
  piece = aurora->white_king;
  int i = __builtin_ctzll(piece);
  uint64_t king_moves;

  if (i > 9)
    king_moves = KING_SPAN << (i - 9) & not_white_pieces;
  else
    king_moves = KING_SPAN >> (9 - i) & not_white_pieces;

  if (piece & FILE_A)
    king_moves &= ~FILE_H;
  else if (piece & FILE_H)
    king_moves &= ~FILE_A;

  uint64_t move = king_moves & ~(king_moves - 1);
  while (move) {
    Move king_move = {
        &aurora->white_king, piece | move, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL};

    if (move & black_pieces)
      king_move.capture = move;

    moves[nb_moves++] = king_move;

    // LOOP THROUGH THE KING MOVES
    king_moves ^= move;
    move = king_moves & ~(king_moves - 1);
  }

  ///////////////////////////////////////////////////////// WHITE KNIGHTS MOVES
  uint64_t knights = aurora->white_knights;
  piece = knights & ~(knights - 1);
  uint64_t knights_moves;

  while (piece) {
    int i = __builtin_ctzll(piece);

    if (i > 18)
      knights_moves = KNIGHT_SPAN << (i - 18) & not_white_pieces;
    else
      knights_moves = KNIGHT_SPAN >> (18 - i) & not_white_pieces;

    if (piece & FILE_AB)
      knights_moves &= ~FILE_GH;
    else if (piece & FILE_GH)
      knights_moves &= ~FILE_AB;

    uint64_t move = knights_moves & ~(knights_moves - 1);
    while (move) {
      Move knight_move = {&aurora->white_knights,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & black_pieces)
        knight_move.capture = move;

      moves[nb_moves++] = knight_move;

      // LOOP THROUGH THE KNIGHTS MOVES
      knights_moves ^= move;
      move = knights_moves & ~(knights_moves - 1);
    }

    // LOOP THROUGH KNIGHTS
    knights ^= piece;
    piece = knights & ~(knights - 1);
  }

  /////////////////////////////////////////////////////////// WHITE PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->white_pawns << 8) & empty & ~RANK_8;
  piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
      (aurora->white_pawns << 16) & RANK_4 & empty & (empty << 8);
  piece = double_forward_moves & ~(double_forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
      moves[nb_moves++] = (Move){&aurora->white_pawns,
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
      moves[nb_moves++] = (Move){&aurora->white_pawns,
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
  uint64_t promotion_forward = (aurora->white_pawns << 8) & RANK_8 & empty;
  piece = promotion_forward & ~(promotion_forward - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 9),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
                               piece | (piece >> 7),
                               piece,
                               0ULL,
                               piece,
                               &aurora->white_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->white_pawns,
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

  return nb_moves;
}

int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces,
                           uint64_t empty) {
  int nb_moves = 0;

  ////////////////////////////////////////////////////////// BLACK CASTLING
  // KING SIDE CASTLE
  if (aurora->castling_right[2] && 0x2000000000000000ULL & empty &&
      0x4000000000000000ULL & empty) {
    moves[nb_moves++] = (Move){
        &aurora->black_king,   0x5000000000000000ULL, 0ULL, 0ULL, 0ULL, 0ULL,
        0xA000000000000000ULL, &aurora->black_rooks};
  }

  // QUEEN SIDE CASTLE
  if (aurora->castling_right[3] && 0x200000000000000ULL & empty &&
      0x400000000000000ULL & empty && 0x800000000000000ULL & empty) {
    moves[nb_moves++] = (Move){
        &aurora->black_king,  0x1400000000000000ULL, 0ULL, 0ULL, 0ULL, 0ULL,
        0x900000000000000ULL, &aurora->black_rooks};
  }

  ////////////////////////////////////////////////////////// BLACK QUEENS MOVES
  uint64_t queens = aurora->black_queens;
  uint64_t piece = queens & ~(queens - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t queens_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_backslash[diag_index[i][1]]) |
         hyperbola_quintessence(piece, aurora->all_pieces, rank_mask(i)) |
         hyperbola_quintessence(piece, aurora->all_pieces, file_mask(i))) &
        not_black_pieces;

    uint64_t move = queens_attacks & ~(queens_attacks - 1);
    while (move) {
      Move queen_move = {&aurora->black_queens,
                         piece | move,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL,
                         0ULL};

      if (move & white_pieces)
        queen_move.capture = move;

      moves[nb_moves++] = queen_move;

      // LOOP THROUGH THE QUEEN MOVES
      queens_attacks ^= move;
      move = queens_attacks & ~(queens_attacks - 1);
    }

    queens ^= piece;
    piece = queens & ~(queens - 1);
  }

  ////////////////////////////////////////////////////////// BLACK ROOKS MOVES
  uint64_t rooks = aurora->black_rooks;
  piece = rooks & ~(rooks - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t rooks_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces, rank_mask(i)) |
         hyperbola_quintessence(piece, aurora->all_pieces, file_mask(i))) &
        not_black_pieces;

    uint64_t move = rooks_attacks & ~(rooks_attacks - 1);
    while (move) {
      Move rook_move = {&aurora->black_rooks,
                        piece | move,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL,
                        0ULL};

      if (move & white_pieces)
        rook_move.capture = move;

      moves[nb_moves++] = rook_move;

      // LOOP THROUGH THE ROOKS MOVES
      rooks_attacks ^= move;
      move = rooks_attacks & ~(rooks_attacks - 1);
    }

    rooks ^= piece;
    piece = rooks & ~(rooks - 1);
  }

  ////////////////////////////////////////////////////////// BLACK BISHOPS MOVES
  uint64_t bishops = aurora->black_bishops;
  piece = bishops & ~(bishops - 1);

  while (piece) {
    int i = __builtin_ctzll(piece);
    uint64_t bishops_attacks =
        (hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_slash[diag_index[i][0]]) |
         hyperbola_quintessence(piece, aurora->all_pieces,
                                diag_backslash[diag_index[i][1]])) &
        not_black_pieces;

    uint64_t move = bishops_attacks & ~(bishops_attacks - 1);
    while (move) {
      Move bishop_move = {&aurora->black_bishops,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & white_pieces)
        bishop_move.capture = move;

      moves[nb_moves++] = bishop_move;

      // LOOP THROUGH THE BISHOPS MOVES
      bishops_attacks ^= move;
      move = bishops_attacks & ~(bishops_attacks - 1);
    }

    bishops ^= piece;
    piece = bishops & ~(bishops - 1);
  }

  ////////////////////////////////////////////////////////// BLACK KING MOVES
  piece = aurora->black_king;
  int i = __builtin_ctzll(piece);
  uint64_t king_moves;

  if (i > 9)
    king_moves = KING_SPAN << (i - 9) & not_black_pieces;
  else
    king_moves = KING_SPAN >> (9 - i) & not_black_pieces;

  if (piece & FILE_A)
    king_moves &= ~FILE_H;
  else if (piece & FILE_H)
    king_moves &= ~FILE_A;

  uint64_t move = king_moves & ~(king_moves - 1);
  while (move) {
    Move king_move = {
        &aurora->black_king, piece | move, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL};

    if (move & white_pieces)
      king_move.capture = move;

    moves[nb_moves++] = king_move;

    // LOOP THROUGH THE KING MOVES
    king_moves ^= move;
    move = king_moves & ~(king_moves - 1);
  }

  ////////////////////////////////////////////////////////// BLACK KNIGHTS MOVES
  uint64_t knights = aurora->black_knights;
  piece = knights & ~(knights - 1);
  uint64_t knights_moves;

  while (piece) {
    int i = __builtin_ctzll(piece);

    if (i > 18)
      knights_moves = KNIGHT_SPAN << (i - 18) & not_black_pieces;
    else
      knights_moves = KNIGHT_SPAN >> (18 - i) & not_black_pieces;

    if (piece & FILE_AB)
      knights_moves &= ~FILE_GH;
    else if (piece & FILE_GH)
      knights_moves &= ~FILE_AB;

    uint64_t move = knights_moves & ~(knights_moves - 1);
    while (move) {
      Move knight_move = {&aurora->black_knights,
                          piece | move,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL,
                          0ULL};

      if (move & white_pieces)
        knight_move.capture = move;

      moves[nb_moves++] = knight_move;

      // LOOP THROUGH MOVES FOR A KNIGHT
      knights_moves ^= move;
      move = knights_moves & ~(knights_moves - 1);
    }

    // LOOP THROUGH KNIGHTS
    knights ^= piece;
    piece = knights & ~(knights - 1);
  }

  //////////////////////////////////////////////////////////// BLACK PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->black_pawns >> 8) & empty & ~RANK_1;
  piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
      (aurora->black_pawns >> 16) & RANK_5 & empty & (empty >> 8);
  piece = double_forward_moves & ~(double_forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
      moves[nb_moves++] = (Move){&aurora->black_pawns,
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
      moves[nb_moves++] = (Move){&aurora->black_pawns,
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
  uint64_t promotion_forward = (aurora->black_pawns >> 8) & RANK_1 & empty;
  piece = promotion_forward & ~(promotion_forward - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 8),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 9),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_knights,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_bishops,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
                               piece | (piece << 7),
                               0ULL,
                               0ULL,
                               piece,
                               &aurora->black_rooks,
                               0ULL,
                               0ULL};
    moves[nb_moves++] = (Move){&aurora->black_pawns,
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

  return nb_moves;
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

int aurora_get_moves(Aurora *aurora, Move moves[100]) {
  int nb_moves = 0;

  aurora->all_white_pieces = aurora->white_pawns | aurora->white_knights |
                             aurora->white_bishops | aurora->white_rooks |
                             aurora->white_queens;

  aurora->all_black_pieces = aurora->black_pawns | aurora->black_knights |
                             aurora->black_bishops | aurora->black_rooks |
                             aurora->black_queens;

  aurora->all_pieces = aurora->all_white_pieces | aurora->all_black_pieces |
                       aurora->white_king | aurora->black_king;

  uint64_t empty = ~aurora->all_pieces;

  if (aurora->turn) {
    uint64_t not_white_pieces =
        ~(aurora->all_white_pieces | aurora->white_king | aurora->black_king);
    uint64_t black_pieces = aurora->all_black_pieces;

    nb_moves = aurora_get_white_moves(aurora, moves, not_white_pieces,
                                      black_pieces, empty);
  } else {
    uint64_t not_black_pieces =
        ~(aurora->all_black_pieces | aurora->black_king | aurora->white_king);
    uint64_t white_pieces = aurora->all_white_pieces;

    nb_moves = aurora_get_black_moves(aurora, moves, not_black_pieces,
                                      white_pieces, empty);
  }

  return nb_moves;
}
