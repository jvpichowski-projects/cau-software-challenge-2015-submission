/* 
 * File:   tools.h
 * Author: jan
 *
 * Created on 15. Februar 2015, 20:41
 */

#ifndef TOOLS_H
#define	TOOLS_H

#include <iostream>
#include <cstdlib>
#include <limits.h>

#include "constants.h"
#include "globals.h"

namespace Tools
{
    int bitScanReverse(u_int64_t bb);
    
    void printField(u_int64_t field);
    //void printBoard(Board *board);
    u_int64_t set(u_int64_t field, int pos);
    u_int64_t clear(u_int64_t field, int pos);
    bool isSet(u_int64_t field, int pos);

    int getPos(int x, int y);
    int* getPos(int pos);//{x,y}

    bool isInvalid(int pos);
    int opponent(int id);
    
    u_int64_t genMoveField(int pos, u_int64_t used);
    u_int64_t getMoveField(int pos, u_int64_t used);

    int bitScanForward(u_int64_t bb);
    int popCount (u_int64_t x);
    int fastPopCount (u_int64_t x);
    int* bitScan (u_int64_t x, int *length);
    int* fastBitScan (u_int64_t x, int *length);
    
//    int calcBitsSet(u_int64_t value);
//    int* getBitsSet(u_int64_t value);
//    int countBitsSet(u_int64_t value);
    
}

#endif	/* TOOLS_H */

