#pragma once

#include "aurora.h"
#include <stdint.h>

// FUNCTIONS
int aurora_get_moves(Aurora *aurora, Move moves[100]);
int aurora_get_white_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_white_pieces, uint64_t black_pieces);
int aurora_get_black_moves(Aurora *aurora, Move moves[100],
                           uint64_t not_black_pieces, uint64_t white_pieces);

void get_queens_moves(Move moves[100], int *nb_moves, uint64_t *this_side_queens, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied);

void get_rooks_moves(Move moves[100], int *nb_moves, uint64_t *this_side_rooks, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied);

void get_bishops_moves(Move moves[100], int *nb_moves, uint64_t *this_side_bishops, uint64_t not_my_pieces, uint64_t opp_pieces, uint64_t occupied);

void get_knights_moves(Move moves[100], int *nb_moves, uint64_t *this_side_knights, uint64_t not_my_pieces, uint64_t opp_pieces);

void get_king_moves(Move moves[100], int *nb_moves, uint64_t *this_side_king, uint64_t not_my_pieces, uint64_t opp_pieces);

void get_white_castling_moves(Aurora *aurora, Move moves[100], int *nb_moves);
void get_black_castling_moves(Aurora *aurora, Move moves[100], int *nb_moves);
void get_white_pawns_moves(Aurora *aurora, Move moves[100], int *nb_moves, uint64_t black_pieces);
void get_black_pawns_moves(Aurora *aurora, Move moves[100], int *nb_moves, uint64_t white_pieces);

uint64_t reverse(uint64_t b);
uint64_t hyperbola_quintessence(uint64_t piece, uint64_t occupied, uint64_t mask);
