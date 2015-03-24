/* 
 * File:   evaluation.h
 * Author: jan
 *
 * Created on 26. Februar 2015, 14:47
 */

#ifndef EVALUATION_H
#define	EVALUATION_H

#define Q1_all 0x3C387878ULL
#define Q1_best 0x103000ULL
#define Q1_lim 0x20204078ULL

#define Q2_all 0x3C78787ULL
#define Q2_best 0x30300ULL
#define Q2_lim 0x408087ULL

#define Q3_all 0xF0E1E1C0000000ULL
#define Q3_best 0x40C000000000ULL
#define Q3_lim 0xE0202040000000ULL

#define Q4_all 0xF0F1E1E00000000ULL
#define Q4_best 0x60C0000000000ULL
#define Q4_lim 0x708101000000000ULL

#include "types.h"
#include "constants.h"
#include "globals.h"
#include "tools.h"

namespace Evaluation
{
    typedef int (*ToEvaluate)(int playerId, Board board, bool qsearch);
    extern ToEvaluate evaluate;
    
    int evaluateNormal(int playerId, Board board, bool qsearch);
    int evaluateSetMoves(int playerId, Board board, bool qsearch);
    int preEvaluate();
}

#endif	/* EVALUATION_H */

