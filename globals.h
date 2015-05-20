/* 
 * File:   globals.h
 * Author: jan
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
    namespace Config{
        extern int points;
        
        extern int reachPoints1;
        extern int reachPoints2;
        extern int reachPoints3;
        
//        extern int singleLinePoint1;
//        extern int singleLinePoint2;
//        extern int singleLinePoint3;
        
        extern int allLinePoints1;
        extern int allLinePoints2;
        extern int allLinePoints3;
        
        extern int restrictedReachField1;
        extern int restrictedReachField2;
        extern int restrictedReachField3;
        
//        extern int allRestrictedReachField1;
//        extern int allRestrictedReachField2;
//        extern int allRestrictedReachField3;
    }
    
    extern int pointsRing1;
    
    extern u_int64_t ones;
    extern u_int64_t twos;
    extern u_int64_t threes;
    extern timespec moveReqTime;
    
    extern bool _runningGame;
    
    extern clockid_t clockTime;
    
    extern timespec beginningOther;
    
    extern Board _board;
    
    extern bool tt_enabled;
    
    namespace Log{
        
        extern int globalEvalCount;
        extern int globalCutOff;
        extern int globalNodesTravled;
        
#ifdef move_order_stats
#define MAX_NUMBER_OF_MOVES 64
        
        //move_order[number of possible moves][move number] = how often this was the best move
        extern int move_order[MAX_NUMBER_OF_MOVES][MAX_NUMBER_OF_MOVES];
#ifdef move_cutoff_state
        //move_cutoff[number of possible moves][move number] = how often this move was the cause for a cutoff
        extern int move_cutoff[MAX_NUMBER_OF_MOVES][MAX_NUMBER_OF_MOVES];
#endif
#endif
        
    }
}


#endif	/* GLOBALS_H */

