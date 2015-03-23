/* 
 * File:   sortedMoveMemoryStructs.h
 * Author: jonas
 *
 * Created on 22. März 2015, 01:50
 */

#ifndef SORTEDMOVEMEMORYSTRUCTS_H
#define	SORTEDMOVEMEMORYSTRUCTS_H

#include "types.h"

struct OwnPositionsStruct4
{
    OwnPositionsStruct4() : ownPositions(0), length(0){}
    
    u_int64_t ownPositions;
    Move* sortedmoves;
    int length;
};

struct PointsAndDeepStruct3
{
    PointsAndDeepStruct3() : pointsAndDeepHash(0), length(0){}
    
    u_int64_t pointsAndDeepHash;
    OwnPositionsStruct4* ownPositions;
    int length;
};

struct OpponentPositionsStruct2
{
    OpponentPositionsStruct2() : opponentPositions(0), length(0){}
    
    u_int64_t opponentPositions;
    PointsAndDeepStruct3* pointsAndDeep;
    int length;
};

struct UsedFieldsStruct1
{
    UsedFieldsStruct1() : usedFiels(0), length(0){}
    
    u_int64_t usedFiels;
    OpponentPositionsStruct2* opponentPositions;
    int length;
};

struct ShelfStruct
{
    ShelfStruct() : length(0){}
    
    UsedFieldsStruct1* shelfStuff;
    int length;
};
    
#endif	/* SORTEDMOVEMEMORYSTRUCTS_H */
