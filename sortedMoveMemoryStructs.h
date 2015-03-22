/* 
 * File:   sortedMoveMemoryStructs.h
 * Author: jonas
 *
 * Created on 22. März 2015, 01:50
 */

#ifndef SORTEDMOVEMEMORYSTRUCTS_H
#define	SORTEDMOVEMEMORYSTRUCTS_H

#include "types.h"

struct OwnPositionsStruct
{
    u_int64_t ownPositions;
    Move* sortedmoves;
};

struct PointsAndDeepStruct
{
    u_int64_t pointsAndDeepHash;
    OwnPositionsStruct* ownPositions;
};

struct OpponentPositionsStruct
{
    u_int64_t opponentPositions;
    PointsAndDeepStruct* pointsAndDeep;
};

struct UsedFieldsStruct
{
    u_int64_t usedFiels;
    OpponentPositionsStruct* opponentPositions;
};

struct FirstOpponentMoveStruct
{
    Move firstOpponentMove;
    OpponentPositionsStruct* usedFields;
    int deep;
};
    
#endif	/* SORTEDMOVEMEMORYSTRUCTS_H */

