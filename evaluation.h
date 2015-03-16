/* 
 * File:   evaluation.h
 * Author: jan
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
    typedef int (*ToEvaluate)(int playerId, Board board, bool qsearch);
    extern ToEvaluate evaluate;
    
    int evaluateNormal(int playerId, Board board, bool qsearch);
    int preEvaluate();
}

#endif	/* EVALUATION_H */

