/* 
 * File:   SortedMoveMemory.h
 * Author: jonas
 *
 * Created on 22. März 2015, 02:07
 */

#ifndef SORTEDMOVEMEMORY_H
#define	SORTEDMOVEMEMORY_H

#include "sortedMoveMemoryStructs.h"

namespace SortedMoveMemory
{
    int getShelfPos(Board board);
    Move* getSortedMoves(Board board, bool* inMemory);
    int getOwnPostruct(Board board, int shelfPos, int usedFieldsStructPos, int opPostructPos, int pointsDeepStructPos);
    int getPointsDeepStruct(Board board, int shelfPos, int usedFieldsStructPos, int opPostructPos);
    int getOpponentPositionsStruct(Board board, int shelfPos, int usedFieldsStructPos);
    int getUsedFieldsStructPos(Board board, int shelfPos);
    
    int init();
}

#endif	/* SORTEDMOVEMEMORY_H */

