/* 
 * File:   types.h
 *
 * Created on 15. Februar 2015, 20:38
 */

#ifndef TYPES_H
#define	TYPES_H

#include <stdlib.h>
#include "config.h"
#include "constants.h"

typedef u_int8_t byte;

struct Move {
    u_int8_t from;
    u_int8_t to;
    int value;
    Move(): value(MIN_AB_VALUE-10){}
};

struct Board {
    
    Board() : used(0), turn(0), pointsdiff(0), movecount(0), mypos(0), oppos(0)
//    , 
//    fastValue(0), lastMove(Move()),
//    pos({{0,0,0,0},{0,0,0,0}}), moveFieldAll({0ULL,0ULL}), moveField({{0ULL, 0ULL, 0ULL, 0ULL},{0ULL, 0ULL, 0ULL, 0ULL}})
    {}
    
    //60
    unsigned long long used:60;
    //1
    unsigned turn:1;
    //3bits unused
    //60
    unsigned long long mypos:60;
    //60
    unsigned long long oppos:60;
    //7 (1 bit sign 6 bits number) -64 to +63
    signed short pointsdiff:7;
    //6
    unsigned movecount:6;
    
    bool operator==(const Board& rhs) const
    {
       return (used == rhs.used)
               && (movecount == rhs.movecount)
               && (pointsdiff == rhs.pointsdiff)
               && (turn == rhs.turn);
    }
    bool operator!=(const Board& rhs) const
    {
      return !operator==(rhs);
    }
};

#endif	/* TYPES_H */

