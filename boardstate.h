/* 
 * File:   state.h
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
    
    /**
     * Apply a move to the board. The board will be changed.
     * 
     * @param state
     * @param playerId
     * @param move
     * @return the movecount of that board after the change
     */
    int apply(Board* state, int playerId, Move move);
    
    /**
     * Remove a move from the board. The board will be changed.
     * 
     * @param state
     * @param playerId
     * @param move
     * @return the movecount of that board after the change
     */
    int unapply(Board* state, int playerId, Move move);
    
    /**
     * Check if a move is valid for the give state.
     * 
     * @param state
     * @param move
     * @param player
     * @return 
     */
    bool isValidMove(Board state, Move move, int player);

    /**
     * Generate a list of possible moves for the give state. This must not 
     * always be all possible moves. It could only contain the best moves.
     * 
     * @param state
     * @param playerId
     * @param length the length of the move list
     * @return The list of the moves
     */
    Move* generatePossibleMoves(Board state, int playerId, int *length);
    
    /**
     * Generates a good move. It will probably not be the best but a good enough 
     * move.
     * 
     * @param state
     * @param playerId
     * @return the move 
     */
    Move generateGoodMove(Board state, int playerId);
};

#endif	/* STATE_H */

