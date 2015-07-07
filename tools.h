/* 
 * File:   tools.h
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
    
    /**
     * Print the formated field.
     * 
     * @param field
     */
    void printField(u_int64_t field);

    /**
     * Returns the bitfield pos of the coordinate pos
     * 
     * @param x
     * @param y
     * @return 
     */
    int getPos(int x, int y);
    
    /**
     * Returns the coordinate pos of the bitfield pos
     * @param pos
     * @return 
     */
    int* getPos(int pos);//{x,y}

    /**
     * Check if a bitfield pos is not >= 60
     * 
     * @param pos
     * @return 
     */
    bool isInvalid(int pos);
    
    /**
     * Generates a field of reachable positions from pos in constant time.
     * 
     * @param pos
     * @param used
     * @return 
     */
    u_int64_t genMoveField(int pos, u_int64_t used);

    /**
     * 
     * @param bb
     * @return 
     */
    int bitScanForward(u_int64_t bb);
    
    /**
     * Counts the number of set bits in constant time.
     * 
     * @param x
     * @return 
     */
    int popCount (u_int64_t x);
    
    /**
     * Counts the number of set bits in O(n).
     * 
     * @param x
     * @return 
     */
    int fastPopCount (u_int64_t x);
    
    /**
     * Returns an array of set bits based on popCount.
     * 
     * @param x
     * @param length the length of the resulting array
     * @return 
     */
    int* bitScan (u_int64_t x, int *length);
    
    /**
     * Returns an array of set bits based on fastPopCount.
     * 
     * @param x
     * @param length the length of the resulting array
     * @return 
     */
    int* fastBitScan (u_int64_t x, int *length);
    
    /**
     * Returns a field of reachable positions for both players.
     * It is based on genMoveField and adds them.
     * 
     * @param field
     * @param posa1
     * @param posa2
     * @param posa3
     * @param posa4
     * @param posb1
     * @param posb2
     * @param posb3
     * @param posb4
     * @param resulta
     * @param resultb
     */
    void getReachableFields(u_int64_t field, 
            int posa1, int posa2, int posa3, int posa4,  
            int posb1, int posb2, int posb3, int posb4, 
            u_int64_t *resulta, u_int64_t *resultb);
    
    
}

#endif	/* TOOLS_H */

