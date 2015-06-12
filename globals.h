/* 
 * File:   globals.h
 *
 * Created on 15. Februar 2015, 20:44
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#include <stdlib.h>
#include <ctime>
#include "types.h"
#include "config.h"

namespace Globals{
    
    extern int pointsRing1;
    
    extern u_int64_t ones;
    extern u_int64_t twos;
    extern u_int64_t threes;
    extern timespec moveReqTime;
    
    extern bool _runningGame;
    
    extern clockid_t clockTime;
    
    extern timespec beginningOther;
    
    extern Board _board;
}


#endif	/* GLOBALS_H */

