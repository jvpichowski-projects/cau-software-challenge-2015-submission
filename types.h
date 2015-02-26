/* 
 * File:   types.h
 * Author: jan
 *
 * Created on 15. Februar 2015, 20:38
 */

#ifndef TYPES_H
#define	TYPES_H

#include <stdlib.h>

typedef u_int8_t byte;


//Move is a number from 0 to 59
//if(move.to >= 60){
//  Nullmove         
//}else if(move.from >= 60){
//  Setmove
//}else{
//  Normalmove
//}
struct Move {
    u_int8_t from;
    u_int8_t to;
};

namespace TT{
    struct Entry{
        u_int64_t hash;
        Move best;
        int score;
        byte type;
        byte depth;
        Entry() : depth(-1), hash(0ULL) {} //66 not reachable
    };
}

struct Board {
    
    Board() : used(0), turn(0), pointsdiff(0), movecount(0),
                    mypos(0), oppos(0){}
    
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
    signed pointsdiff:7;
    //6
    unsigned movecount:6;
    //4bits unused
    
};


#endif	/* TYPES_H */

