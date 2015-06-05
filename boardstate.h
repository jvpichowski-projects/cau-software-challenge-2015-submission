/* 
 * File:   state.h
 * Author: jan
 *
 * Created on 17. Februar 2015, 17:32
 */

#ifndef STATE_H
#define	STATE_H

#include <algorithm>

#include "types.h"
#include "constants.h"
#include "globals.h"
#include "tools.h"
#include "config.h"

namespace BoardTools{
    int apply(Board* state, int playerId, Move move);
    int unapply(Board* state, int playerId, Move move);
    bool isValidMove(Board state, Move move, int player);

//    void genMoveField(Board* board);
//    Board* generatePossibleBoards(Board state, int playerId, int* length);
    Move* generatePossibleMoves(Board state, int playerId, int *length);
    Move generateGoodMove(Board state, int playerId);
};

#endif	/* STATE_H */

