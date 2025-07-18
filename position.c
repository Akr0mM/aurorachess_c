#include "position.h"
#include "aurora.h"
#include "board.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void aurora_make_move(Aurora *aurora, Move move) {
  // REMOVE THE PIECE FROM THE BOARD_BITBOARD
  aurora->board_bitboard[__builtin_ctzll(aurora->bitboards[move.piece] & move.move)] = NONE;

  // MAKING THE MOVE ON THE BITBOARD
  aurora->bitboards[move.piece] ^= move.move;

  if (move.promotion) {
    // REMOVE PAWN THAT PROMOTES
    aurora->bitboards[move.piece] ^= move.promotion;

    // ADD CREATED PIECE
    aurora->bitboards[move.promotion_piece] |= move.promotion;
  }

  if (move.castle) {
    aurora->bitboards[move.rook] ^= move.castle;
  }

  if (move.en_passant_piece_captured != NONE) {
    // REMOVE THE CAPTURE PAWN
    aurora->bitboards[move.en_passant_piece_captured] ^= move.capture;
  } else if (move.capture) {
    // REMOVE THE CAPTURED PIECE
    Piece piece_captured = get_piece_captured(aurora, move.capture);
    aurora->bitboards[piece_captured] ^= move.capture;
  }

  // ADDING ENPASSANT SQUARE IF IS A DOUBLE FORWARD PAWN MOVE
  if ((move.piece == WP || move.piece == BP) &&
      move.move >> __builtin_ctzll(move.move) == DOUBLE_FORWARD_PAWN_SPAN) {
    aurora->en_passant = 1ULL << (__builtin_ctzll(move.move) + 8);
  } else {
    aurora->en_passant = 0ULL;
  }

  // ADDING THE &BITBOARD TO THE NEW SQUARE WHERE THE PIECE LANDS
  if (move.promotion) {
    aurora->board_bitboard[__builtin_ctzll(move.promotion)] =
        move.promotion_piece;
  } else {
    aurora->board_bitboard[__builtin_ctzll(aurora->bitboards[move.piece] & move.move)] =
        move.piece;
  }

  // PASSING TO NEXT BOARD
  aurora->turn = !aurora->turn;
}

void aurora_undo_move(Aurora *aurora) { (void)aurora; }

void aurora_analyse(Aurora *aurora) {
  aurora->bitboards[WHITE] = aurora->bitboards[WP] | aurora->bitboards[WN] |
                             aurora->bitboards[WB] | aurora->bitboards[WR] |
                             aurora->bitboards[WQ] | aurora->bitboards[WK];

  aurora->bitboards[BLACK] = aurora->bitboards[BP] | aurora->bitboards[BN] |
                             aurora->bitboards[BB] | aurora->bitboards[BR] |
                             aurora->bitboards[BQ] | aurora->bitboards[BK];

  aurora->bitboards[ALL] = aurora->bitboards[WHITE] | aurora->bitboards[BLACK];

  aurora->bitboards[EMPTY] = ~aurora->bitboards[ALL];  
}

Move get_move_from_sq(Move moves[100], char sq[5]) {
  uint64_t mask = convert_sq_to_mask(sq);

  for (int i = 0; i < 100; i++) {
    if (moves[i].move == mask) {
      switch (sq[4]) {
      case '\0':
        return moves[i];
      case 'n':
        return moves[i];
      case 'b':
        return moves[i + 1];
      case 'r':
        return moves[i + 2];
      case 'q':
        return moves[i + 3];
      }
    }
  }

  printf("BAD BAD BAD BAD BAD BAD BAD BAD BAD BAD POSITION MOVE NOT FOUND "
         "!!!!!!!!\n");
  return (Move){};
}

uint64_t convert_sq_to_mask(char sq[4]) {
  int i1 = (sq[0] - 'a') + (sq[1] - '1') * 8;
  int i2 = (sq[2] - 'a') + (sq[3] - '1') * 8;

  return (1ULL << i1) | (1ULL << i2);
}

uint64_t file_mask(int i) {
  int r = i % 8;
  switch (r) {
  case 0:
    return FILE_A;
  case 1:
    return FILE_B;
  case 2:
    return FILE_C;
  case 3:
    return FILE_D;
  case 4:
    return FILE_E;
  case 5:
    return FILE_F;
  case 6:
    return FILE_G;
  case 7:
    return FILE_H;
  default:
    return 0ULL;
  }
}
uint64_t rank_mask(int i) {
  int q = (i - (i % 8)) / 8;
  switch (q) {
  case 0:
    return RANK_1;
  case 1:
    return RANK_2;
  case 2:
    return RANK_3;
  case 3:
    return RANK_4;
  case 4:
    return RANK_5;
  case 5:
    return RANK_6;
  case 6:
    return RANK_7;
  case 7:
    return RANK_8;
  default:
    return 0ULL;
  }
}

Piece get_piece_captured(Aurora *aurora, uint64_t mask) {
  return aurora->board_bitboard[__builtin_ctzll(mask)];
}

void aurora_load_fen(Aurora *aurora, char *fen) {
  char fenbuf[128];
  strncpy(fenbuf, fen, sizeof fenbuf);
  fenbuf[sizeof fenbuf - 1] = '\0';

  char *tokens[6];
  int ntok = 0;
  char *save;

  for (char *p = strtok_r(fenbuf, " ", &save); p && ntok < 6;
       p = strtok_r(NULL, " ", &save)) {
    tokens[ntok++] = p;
  }

  // turn
  aurora->turn = strcmp(tokens[1], "w") == 0 ? 1 : 0;

  // castlings
  for (size_t i = 0; i < strlen(tokens[2]); i++) {
    switch (tokens[2][i]) {
    case 'K':
      aurora->castling_right[0] = 1;
      break;
    case 'Q':
      aurora->castling_right[1] = 1;
      break;
    case 'k':
      aurora->castling_right[2] = 1;
      break;
    case 'q':
      aurora->castling_right[3] = 1;
      break;
    }
  }

  // en passant
  aurora->en_passant =
      strcmp(tokens[3], "-") == 0 ? 0ULL : square_to_bitboard(tokens[3]);

  uint64_t pos = 1ULL << 56;
  int file = 0;
  for (size_t i = 0; i < strlen(tokens[0]); i++) {
    char c = tokens[0][i];
    if (c == '/') {
      pos >>= file == 8 ? 16 : 15;
      file = 0;
      continue;
    } else if (c >= '1' && c <= '8') {
      int skip = c - '1';
      pos <<= skip;
      file += skip;

      if (file != 7) {
        pos <<= 1;
        file++;
      }
      continue;
    }

    switch (c) {
    case 'P':
      aurora->bitboards[WP] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WP;
      break;
    case 'N':
      aurora->bitboards[WN] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WN;
      break;
    case 'B':
      aurora->bitboards[WB] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WB;
      break;
    case 'R':
      aurora->bitboards[WR] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WR;
      break;
    case 'Q':
      aurora->bitboards[WQ] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WQ;
      break;
    case 'K':
      aurora->bitboards[WK] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = WK;
      break;
    case 'p':
      aurora->bitboards[BP] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BP;
      break;
    case 'n':
      aurora->bitboards[BN] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BN;
      break;
    case 'b':
      aurora->bitboards[BB] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BB;
      break;
    case 'r':
      aurora->bitboards[BR] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BR;
      break;
    case 'q':
      aurora->bitboards[BQ] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BQ;
      break;
    case 'k':
      aurora->bitboards[BK] |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = BK;
      break;
    }

    if (file != 7) {
      pos <<= 1;
      file++;
    }
  }
}

uint64_t square_to_bitboard(char square[2]) {
  return 1ULL <<  ((square[1] - '1') * 8 + square[0] - 'a');
}
