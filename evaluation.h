/* 
 * File:   evaluation.h
 *
 * Created on 26. Februar 2015, 14:47
 */

#ifndef EVALUATION_H
#define	EVALUATION_H

#include "types.h"
#include "constants.h"
#include "globals.h"
#include "tools.h"

namespace Evaluation
{
    //type of evaluation method
    typedef int (*ToEvaluate)(int playerId, Board board);
    
    /**
     * This method evaluates the current game state and returns a number how 
     * likely the give player will win the game.
     */
    extern ToEvaluate evaluate;
    
    /**
     * Like evaluate only faster and leaves out some evaluation criteria.
     */
    extern ToEvaluate fastEvaluate;
    
    /**
     * This method should be called before alpha-beta is started.
     * It activates the right evaluation methods for the given moveCount.
     * 
     * @param moveCount
     * @return 
     */
    int preEvaluate(int moveCount);
}

#endif	/* EVALUATION_H */

