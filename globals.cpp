#include "globals.h"

namespace Globals{
    
    namespace Config{
        int points = 4;
        int moveFields = 1;
        int ringFields = 1;
        int rReachFields = 1;
        int aReachFields = 1;
        
//        int reachPoints1 = 1;
//        int reachPoints2 = 2;
//        int reachPoints3 = 4;
        
//        int singleLinePoint1 = 1;//5;
//        int singleLinePoint2 = 2;//5*2;
//        int singleLinePoint3 = 4;//5*3;
        
//        int allLinePoints1 = 1;//1;
//        int allLinePoints2 = 2;//1*2;
//        int allLinePoints3 = 4;//1*3;
//        
//        int restrictedReachField1 = 1;
//        int restrictedReachField2 = 2;
//        int restrictedReachField3 = 4;
        
//        int allRestrictedReachField1 = 1;
//        int allRestrictedReachField2 = 2;
//        int allRestrictedReachField3 = 4;
        
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