#pragma once

#include "aurora.h"
#include <sys/types.h>

// FUNCTION
void aurora_load_fen(Aurora *aurora, char *fen);
u_int64_t convert_sq_to_mask(char sq[4]);
Move get_move_from_sq(Move moves[100], char sq[4]);
void aurora_make_move(Aurora *aurora, Move move);
void aurora_undo_move(Aurora *aurora);
uint64_t *get_piece_captured(Aurora *aurora, uint64_t mask);
uint64_t file_mask(int i);
uint64_t rank_mask(int i);
