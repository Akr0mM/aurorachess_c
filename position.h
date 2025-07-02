#pragma once

#include "aurora.h"

// FUNCTION
void aurora_load_fen(Aurora *aurora, char *fen);
void make_move(Aurora *aurora, Move move);
void undo_move(Aurora *aurora);
uint64_t *get_piece_captured(Aurora *aurora, uint64_t mask);
