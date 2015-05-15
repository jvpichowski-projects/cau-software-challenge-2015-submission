#include "globals.h"

namespace Globals{
    
    namespace Config{
        int points = 1;
        int reachPoints = 1;
    }
    
    int pointsRing1;
    
    u_int64_t ones;
    u_int64_t twos;
    u_int64_t threes;
    timespec moveReqTime;
    
    bool _runningGame = true;
    
    clockid_t clockTime = CLOCK_MONOTONIC; 
    
    timespec beginningOther;
    
    Board _board;
    
    bool tt_enabled = true;
    
    namespace Log{
        
        int globalEvalCount;
        int globalCutOff;
        int globalNodesTravled;
        
#ifdef move_order_stats
        int move_order[64][64];
#ifdef move_cutoff_state
        int move_cutoff[64][64];
#endif
#endif
        
    }
}