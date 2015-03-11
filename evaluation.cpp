#include "evaluation.h"

//int64_t _fieldsAround[60] = {
//            /* |  00 */0x182ULL,             /* |  01 */0x305ULL,             /* |  02 */0x60AULL,             /* |  03 */0xC14ULL,
//            /* |  04 */0x1828ULL,            /* |  05 */0x3050ULL,            /* |  06 */0x6020ULL,            /* |  07 */0x8101ULL,
//            /* |  08 */0x18283ULL,           /* |  09 */0x30506ULL,           /* |  10 */0x60A0CULL,           /* |  11 */0xC1418ULL,
//            /* |  12 */0x182830ULL,          /* |  13 */0x305060ULL,          /* |  14 */0x202040ULL,          /* |  15 */0xC10180ULL,
//            /* |  16 */0x1828300ULL,         /* |  17 */0x3050600ULL,         /* |  18 */0x60A0C00ULL,         /* |  19 */0xC141800ULL,
//            /* |  20 */0x18283000ULL,        /* |  21 */0x30106000ULL,        /* |  22 */0x40808000ULL,        /* |  23 */0xC1418000ULL,
//            /* |  24 */0x182830000ULL,       /* |  25 */0x305060000ULL,       /* |  26 */0x60A0C0000ULL,       /* |  27 */0xC14180000ULL,
//            /* |  28 */0x1828300000ULL,      /* |  29 */0x1010200000ULL,      /* |  30 */0x6080C00000ULL,      /* |  31 */0xC141800000ULL,
//            /* |  32 */0x18283000000ULL,     /* |  33 */0x30506000000ULL,     /* |  34 */0x60A0C000000ULL,     /* |  35 */0xC1418000000ULL,
//            /* |  36 */0x180830000000ULL,    /* |  37 */0x204040000000ULL,    /* |  38 */0x60A0C0000000ULL,    /* |  39 */0xC14180000000ULL,
//            /* |  40 */0x1828300000000ULL,   /* |  41 */0x3050600000000ULL,   /* |  42 */0x60A0C00000000ULL,   /* |  43 */0xC141800000000ULL,
//            /* |  44 */0x8081000000000ULL,   /* |  45 */0x30406000000000ULL,  /* |  46 */0x60A0C000000000ULL,  /* |  47 */0xC1418000000000ULL,
//            /* |  48 */0x182830000000000ULL, /* |  49 */0x305060000000000ULL, /* |  50 */0x60A0C0000000000ULL, /* |  51 */0xC04180000000000ULL,
//            /* |  52 */0x20200000000000ULL,  /* |  53 */0x50600000000000ULL,  /* |  54 */0xA0C00000000000ULL,  /* |  55 */0x141800000000000ULL,
//            /* |  56 */0x283000000000000ULL, /* |  57 */0x506000000000000ULL, /* |  58 */0xA0C000000000000ULL, /* |  59 */0x408000000000000ULL   };

int evaluate(int playerId, Board board){
    int points = board.pointsdiff;
        
    if(board.movecount >= 60){
        if(playerId != ID_WE){
            return -points;
        }
        return points;
    }

    int l = 0;
    int *penguinPos = Tools::fastBitScan(board.mypos, &l);

    //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
    u_int64_t moveFields = Tools::getMoveField(penguinPos[0], board.used) 
            | Tools::getMoveField(penguinPos[1], board.used) 
            | Tools::getMoveField(penguinPos[2], board.used)
            | Tools::getMoveField(penguinPos[3], board.used);


    int movePoints = 0;
    movePoints += Tools::popCount(moveFields & Globals::threes) * 8;
    movePoints += Tools::popCount(moveFields & Globals::twos) * 4;
    movePoints += Tools::popCount(moveFields & Globals::ones) * 2;
    
    u_int64_t bigmovefield = 0ULL;        
    int len;
    int* posis = Tools::bitScan(moveFields, &len);

    for(u_int8_t i = 0; i < len; i++)
        bigmovefield |= Tools::getMoveField(posis[i], board.used);
    
    movePoints += Tools::popCount(bigmovefield & Globals::threes) * 4;
    movePoints += Tools::popCount(bigmovefield & Globals::twos) * 2;
    movePoints += Tools::popCount(bigmovefield & Globals::ones);


    delete[] penguinPos, posis;

    penguinPos = Tools::fastBitScan(board.oppos, &l);

    //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
    moveFields = Tools::getMoveField(penguinPos[0], board.used) 
            | Tools::getMoveField(penguinPos[1], board.used) 
            | Tools::getMoveField(penguinPos[2], board.used)
            | Tools::getMoveField(penguinPos[3], board.used);

    movePoints -= Tools::popCount(moveFields & Globals::threes) * 8;
    movePoints -= Tools::popCount(moveFields & Globals::twos) * 4;
    movePoints -= Tools::popCount(moveFields & Globals::ones) * 2;

    
    bigmovefield = 0ULL;        
    int len2;
    posis = Tools::bitScan(moveFields, &len);

    for(u_int8_t i = 0; i < len; i++)
        bigmovefield |= Tools::getMoveField(posis[i], board.used);
    
    movePoints -= Tools::popCount(bigmovefield & Globals::threes) * 4;
    movePoints -= Tools::popCount(bigmovefield & Globals::twos) * 2;
    movePoints -= Tools::popCount(bigmovefield & Globals::ones);
    
//    for(int i = 0; i < l; i++)
//    {
//        if(Tools::popCount(_fieldsAround[penguinPos[i]] & Tools::getMoveField(penguinPos[i], board.used)) < 1)
//            movePoints += 2;
//    }

    delete[] penguinPos, posis;

//        std::cout << "Points: " << points << std::endl;
       
    
    int result = 4 * points + movePoints;
       
        
    if(playerId != ID_WE){
        return -result;
    }
    return result;
}
