/* 
 * File:   tactic.h
 * Author: jonas
 *
 * Created on 7. März 2015, 00:08
 */

#ifndef TACTIC_H
#define	TACTIC_H

#include <stdlib.h>
#include "types.h"
#include "tools.h"
#include "boardstate.h"
#include <stdio.h>





namespace Tactic
{
    
    
    
    int preliminaries(Move* resultMoves, int* resultmoveLng, u_int64_t* activePlayer);
    u_int64_t getBigMoveField(u_int8_t pos, Board board);
    int getAvailablePoints(u_int8_t player, Board board);
    int getOneFieldMove();
    int selectOneFieldMove(int pos);
    int getAvailablePointsDiff(Board board);
    //int getAvailableFields(u_int8_t pos, Board board);
}

#endif	/* TACTIC_H */

