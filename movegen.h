#pragma once

#include "aurora.h"

// FUNCTIONS
int aurora_get_moves(Aurora *aurora, Move moves[100]);
int aurora_get_white_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_white_pieces, uint64_t black_pieces,
                           uint64_t empty);
int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces,
                           uint64_t empty);
