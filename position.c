#include "position.h"
#include "aurora.h"

#include <string.h>

void make_move(Aurora *aurora, Move move) {
  // REMOVE THE &BITBOARD FROM THE BOARD_BITBOARD
  aurora->board_bitboard[__builtin_ctzll(*move.piece & move.move)] = NULL;

  // MAKING THE MOVE ON THE BITBOARD
  *move.piece ^= move.move;

  if (move.promotion) {
    // REMOVE PAWN THAT PROMOTES
    *move.piece ^= move.promotion;

    // ADD CREATED PIECE
    *move.promotion_piece |= move.promotion;
  }

  if (move.en_passant_piece_captured) {
    // REMOVE THE CAPTURE PAWN
    *move.en_passant_piece_captured ^= move.capture;
  } else if (move.capture) {
    // REMOVE THE CAPTURED PIECE
    uint64_t *piece_captured = get_piece_captured(aurora, move.capture);
    *piece_captured ^= move.capture;
  }

  // ADDING ENPASSANT SQUARE IF IS A DOUBLE FORWARD PAWN MOVE
  if ((move.piece == &aurora->white_pawns ||
       move.piece == &aurora->black_pawns) &&
      move.move >> __builtin_ctzll(move.move) == DOUBLE_FORWARD_PAWN_SPAN) {
    aurora->en_passant = 1ULL << (__builtin_ctzll(move.move) + 8);
  }

  // ADDING THE &BITBOARD TO THE NEW SQUARE WHERE THE PIECE LANDS
  if (move.promotion) {
    aurora->board_bitboard[__builtin_ctzll(move.promotion)] =
        move.promotion_piece;
  } else {
    aurora->board_bitboard[__builtin_ctzll(*move.piece & move.move)] =
        move.piece;
  }

  // PASSING TO NEXT BOARD
  aurora->en_passant = 0ULL;
  aurora->turn = !aurora->turn;
}

void undo_move(Aurora *aurora) {}

uint64_t *get_piece_captured(Aurora *aurora, uint64_t mask) {
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
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_pawns |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_pawns;
      break;
    case 'N':
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_knights |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_knights;
      break;
    case 'B':
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_bishops |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_bishops;
      break;
    case 'R':
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_rooks |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_rooks;
      break;
    case 'Q':
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_queens |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_queens;
      break;
    case 'K':
      aurora->all_pieces |= pos;
      aurora->all_white_pieces |= pos;
      aurora->white_king |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->white_king;
      break;
    case 'p':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_pawns |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_pawns;
      break;
    case 'n':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_knights |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_knights;
      break;
    case 'b':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_bishops |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_bishops;
      break;
    case 'r':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_rooks |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_rooks;
      break;
    case 'q':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_queens |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_queens;
      break;
    case 'k':
      aurora->all_pieces |= pos;
      aurora->all_black_pieces |= pos;
      aurora->black_king |= pos;
      aurora->board_bitboard[__builtin_ctzll(pos)] = &aurora->black_king;
      break;
    }

    if (file != 7) {
      pos <<= 1;
      file++;
    }
  }
}
