#include "movegen.h"
#include "aurora.h"

int aurora_get_white_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_white_pieces, uint64_t black_pieces,
                           uint64_t empty) {
  int nb_moves = 0;

  ////////////////////////////////////////////////////////// WHITE KNIGHTS MOVES
  uint64_t knights = aurora->white_knights;
  uint64_t piece = knights & ~(knights - 1);
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
      Move knight_move = {
          &aurora->white_knights, piece | move, 0ULL, 0ULL, 0ULL, 0ULL};

      if (move & black_pieces)
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

  /////////////////////////////////////////////////////////// WHITE PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->white_pawns << 8) & empty & ~RANK_8;
  piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){
        &aurora->white_pawns, piece | (piece >> 8), 0ULL, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->white_pawns, piece | (piece >> 16), 0ULL, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->white_pawns, piece | (piece >> 9), piece, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->white_pawns, piece | (piece >> 7), piece, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 8), 0ULL, 0ULL, piece,
               &aurora->white_knights};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 8), 0ULL, 0ULL, piece,
               &aurora->white_bishops};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 8), 0ULL, 0ULL, piece,
               &aurora->white_rooks};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 8), 0ULL, 0ULL, piece,
               &aurora->white_queens};

    promotion_forward ^= piece;
    piece = promotion_forward & ~(promotion_forward - 1);
  }

  // PROMOTION CAPTURE RIGHT
  uint64_t promotion_capture_right =
      (aurora->white_pawns << 9) & black_pieces & RANK_8 & ~FILE_A;
  piece = promotion_capture_right & ~(promotion_capture_right - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 9), piece, 0ULL, piece,
               &aurora->white_knights};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 9), piece, 0ULL, piece,
               &aurora->white_bishops};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 9), piece, 0ULL, piece,
               &aurora->white_rooks};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 9), piece, 0ULL, piece,
               &aurora->white_queens};

    promotion_capture_right ^= piece;
    piece = promotion_capture_right & ~(promotion_capture_right - 1);
  }

  // PROMOTION CAPTURE LEFT
  uint64_t promotion_capture_left =
      (aurora->white_pawns << 7) & black_pieces & RANK_8 & ~FILE_H;
  piece = promotion_capture_left & ~(promotion_capture_left - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 7), piece, 0ULL, piece,
               &aurora->white_knights};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns,  piece | (piece >> 7), piece, 0ULL, piece,
               &aurora->white_bishops};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 7), piece, 0ULL, piece,
               &aurora->white_rooks};
    moves[nb_moves++] =
        (Move){&aurora->white_pawns, piece | (piece >> 7), piece, 0ULL, piece,
               &aurora->white_queens};

    promotion_capture_left ^= piece;
    piece = promotion_capture_left & ~(promotion_capture_left - 1);
  }

  return nb_moves;
}

int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces,
                           uint64_t empty) {
  int nb_moves = 0;

  ////////////////////////////////////////////////////////// WHITE KNIGHTS MOVES
  uint64_t knights = aurora->black_knights;
  uint64_t piece = knights & ~(knights - 1);
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
      Move knight_move = {
          &aurora->black_knights, piece | move, 0ULL, 0ULL, 0ULL, 0ULL};

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

  //////////////////////////////////////////////////////////// WHITE PAWNS MOVES
  // PAWN MOVE FORWARD 1
  uint64_t forward_moves = (aurora->black_pawns >> 8) & empty & ~RANK_1;
  piece = forward_moves & ~(forward_moves - 1);

  while (piece) {
    // ADD PAWN MOVE
    moves[nb_moves++] = (Move){
        &aurora->black_pawns, piece | (piece << 8), 0ULL, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->black_pawns, piece | (piece << 16), 0ULL, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->black_pawns, piece | (piece << 9), piece, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] = (Move){
        &aurora->black_pawns, piece | (piece << 7), piece, 0ULL, 0ULL, 0ULL};

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
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 8), 0ULL, 0ULL, piece,
               &aurora->black_knights};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 8), 0ULL, 0ULL, piece,
               &aurora->black_bishops};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 8), 0ULL, 0ULL, piece,
               &aurora->black_rooks};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 8), 0ULL, 0ULL, piece,
               &aurora->black_queens};

    promotion_forward ^= piece;
    piece = promotion_forward & ~(promotion_forward - 1);
  }

  // PROMOTION CAPTURE RIGHT
  uint64_t promotion_capture_right =
      (aurora->black_pawns >> 9) & white_pieces & RANK_1 & ~FILE_H;
  piece = promotion_capture_right & ~(promotion_capture_right - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 9), 0ULL, 0ULL, piece,
               &aurora->black_knights};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 9), 0ULL, 0ULL, piece,
               &aurora->black_bishops};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 9), 0ULL, 0ULL, piece,
               &aurora->black_rooks};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 9), 0ULL, 0ULL, piece,
               &aurora->black_queens};

    promotion_capture_right ^= piece;
    piece = promotion_capture_right & ~(promotion_capture_right - 1);
  }

  // PROMOTION CAPTURE LEFT
  uint64_t promotion_capture_left =
      (aurora->black_pawns >> 7) & white_pieces & RANK_1 & ~FILE_A;
  piece = promotion_capture_left & ~(promotion_capture_left - 1);

  while (piece) {
    // MOVE FOR ALL PROMOTION TYPE
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 7), 0ULL, 0ULL, piece,
               &aurora->black_knights};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns,  piece | (piece << 7), 0ULL, 0ULL, piece,
               &aurora->black_bishops};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 7), 0ULL, 0ULL, piece,
               &aurora->black_rooks};
    moves[nb_moves++] =
        (Move){&aurora->black_pawns, piece | (piece << 7), 0ULL, 0ULL, piece,
               &aurora->black_queens};

    promotion_capture_left ^= piece;
    piece = promotion_capture_left & ~(promotion_capture_left - 1);
  }

  return nb_moves;
}

int aurora_get_moves(Aurora *aurora, Move moves[100]) {
  int nb_moves = 0;

  uint64_t empty = ~aurora->all_pieces;

  if (aurora->turn) {
    uint64_t not_white_pieces =
        ~(aurora->all_white_pieces | aurora->black_king);
    uint64_t black_pieces = aurora->all_black_pieces ^ aurora->black_king;

    nb_moves = aurora_get_white_moves(aurora, moves, not_white_pieces,
                                      black_pieces, empty);
  } else {
    uint64_t not_black_pieces =
        ~(aurora->all_black_pieces | aurora->white_king);
    uint64_t white_pieces = aurora->all_white_pieces ^ aurora->white_king;

    nb_moves = aurora_get_black_moves(aurora, moves, not_black_pieces,
                                      white_pieces, empty);
  }

  return nb_moves;
}
