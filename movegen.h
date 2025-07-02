#pragma once

#include "aurora.h"
#include <stdint.h>

// FUNCTIONS
int aurora_get_moves(Aurora *aurora, Move moves[100]);
int aurora_get_white_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_white_pieces, uint64_t black_pieces,
                           uint64_t empty);
int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces,
                           uint64_t empty);
uint64_t reverse(uint64_t b);
uint64_t hyperbola_quintessence(uint64_t piece, uint64_t occupied,
                                uint64_t mask);
