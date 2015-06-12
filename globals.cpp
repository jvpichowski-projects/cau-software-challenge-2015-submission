#include "globals.h"

namespace Globals{
    
    u_int64_t ones;
    u_int64_t twos;
    u_int64_t threes;
    timespec moveReqTime;
    
    bool _runningGame = true;
    
    clockid_t clockTime = CLOCK_MONOTONIC; 
    
    timespec beginningOther;
    
    Board _board;
}