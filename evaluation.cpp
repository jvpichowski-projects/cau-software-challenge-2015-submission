//Ver2

#include "evaluation.h"
#include "boardstate.h"
typedef int PVOID;
int64_t _fieldsAround[60] = {
            /* |  00 */0x182ULL,             /* |  01 */0x305ULL,             /* |  02 */0x60AULL,             /* |  03 */0xC14ULL,
            /* |  04 */0x1828ULL,            /* |  05 */0x3050ULL,            /* |  06 */0x6020ULL,            /* |  07 */0x8101ULL,
            /* |  08 */0x18283ULL,           /* |  09 */0x30506ULL,           /* |  10 */0x60A0CULL,           /* |  11 */0xC1418ULL,
            /* |  12 */0x182830ULL,          /* |  13 */0x305060ULL,          /* |  14 */0x202040ULL,          /* |  15 */0xC10180ULL,
            /* |  16 */0x1828300ULL,         /* |  17 */0x3050600ULL,         /* |  18 */0x60A0C00ULL,         /* |  19 */0xC141800ULL,
            /* |  20 */0x18283000ULL,        /* |  21 */0x30106000ULL,        /* |  22 */0x40808000ULL,        /* |  23 */0xC1418000ULL,
            /* |  24 */0x182830000ULL,       /* |  25 */0x305060000ULL,       /* |  26 */0x60A0C0000ULL,       /* |  27 */0xC14180000ULL,
            /* |  28 */0x1828300000ULL,      /* |  29 */0x1010200000ULL,      /* |  30 */0x6080C00000ULL,      /* |  31 */0xC141800000ULL,
            /* |  32 */0x18283000000ULL,     /* |  33 */0x30506000000ULL,     /* |  34 */0x60A0C000000ULL,     /* |  35 */0xC1418000000ULL,
            /* |  36 */0x180830000000ULL,    /* |  37 */0x204040000000ULL,    /* |  38 */0x60A0C0000000ULL,    /* |  39 */0xC14180000000ULL,
            /* |  40 */0x1828300000000ULL,   /* |  41 */0x3050600000000ULL,   /* |  42 */0x60A0C00000000ULL,   /* |  43 */0xC141800000000ULL,
            /* |  44 */0x8081000000000ULL,   /* |  45 */0x30406000000000ULL,  /* |  46 */0x60A0C000000000ULL,  /* |  47 */0xC1418000000000ULL,
            /* |  48 */0x182830000000000ULL, /* |  49 */0x305060000000000ULL, /* |  50 */0x60A0C0000000000ULL, /* |  51 */0xC04180000000000ULL,
            /* |  52 */0x20200000000000ULL,  /* |  53 */0x50600000000000ULL,  /* |  54 */0xA0C00000000000ULL,  /* |  55 */0x141800000000000ULL,
            /* |  56 */0x283000000000000ULL, /* |  57 */0x506000000000000ULL, /* |  58 */0xA0C000000000000ULL, /* |  59 */0x408000000000000ULL   };

namespace Evaluation
{
    Evaluation::ToEvaluate evaluate;
    
    u_int8_t multpPoints = 3;
    u_int8_t multpMovepo = 1;
    
    u_int8_t multpMovep1 = 1;
    u_int8_t multpMovep2 = 2;
    u_int8_t multpMovep3 = 4;
    
    bool ring1good = true;
    
    u_int8_t multpFiAr = 2;
    
    int preEvaluate()
    {
        //Globals::pointsRing1 = Tools::popCount(RING1 & Globals::threes);
        
//        if(Tools::popCount(RING1 & Globals::threes) < 5)
//        {
//            ring1good = false;
//            std::cout << "\n\n====================================== no = ! !RING1\n\n";
//        }
//        else
//        {
//            std::cout << "\n\n============================================== RING1\n\n";
//        }
        
        if(Globals::_board.movecount < 8)
        {
            evaluate = &Evaluation::evaluateSetMoves;
            
            u_int8_t multpPoints = 3;
            u_int8_t multpMovepo = 1;
        }
        /*else if(Globals::_board.movecount >= 52)
        {
            multpFiAr = 0; 
        }
        else if(Globals::_board.movecount >= 40)
        {
            multpFiAr = 1;
        }
        else if(Globals::_board.movecount >= 34)
        {
            multpPoints = 3;
            multpMovepo = 1;
            
            multpFiAr = 1;
        }
        else if(Globals::_board.movecount >= 20)
        {
            multpPoints = 2;
            multpMovepo = 1;
            
            multpFiAr = 2;
        }*/
        else if(Globals::_board.movecount >= 50)
        {
            multpPoints = 3;
            multpMovepo = 1;
            multpFiAr = 0;
        }
        else if(Globals::_board.movecount >= 38)
        {
            multpPoints = 3;
            multpMovepo = 1;
            multpFiAr = 1;
        }
        else if(Globals::_board.movecount >= 12)
        {
            multpPoints = 2;
            multpMovepo = 1;
            
            multpMovep1 = 1;
            multpMovep2 = 2;
            multpMovep3 = 4;
        }
        else if(Globals::_board.movecount >= 8)
        {
            evaluate = &Evaluation::evaluateNormal;
            
            multpPoints = 4;
            multpMovepo = 1;
            
            multpMovep1 = 0;
            multpMovep2 = 0;
            multpMovep3 = 2;
        }
        
        
        
        
        
    }
    
    int evaluateNormal(int playerId, Board board, bool qsearch)
    {
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
        u_int64_t moveFields = Tools::genMoveField(penguinPos[0], board.used) 
                | Tools::genMoveField(penguinPos[1], board.used) 
                | Tools::genMoveField(penguinPos[2], board.used)
                | Tools::genMoveField(penguinPos[3], board.used);
                
        int movePoints = 0;
        movePoints += Tools::popCount(moveFields & Globals::threes) * multpMovep3;
        movePoints += Tools::popCount(moveFields & Globals::twos) * multpMovep2;
        movePoints += Tools::popCount(moveFields & Globals::ones) * multpMovep1;

        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[0], board.used) & _fieldsAround[penguinPos[0]]);        
        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[1], board.used) & _fieldsAround[penguinPos[1]]);        
        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[2], board.used) & _fieldsAround[penguinPos[2]]);        
        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[3], board.used) & _fieldsAround[penguinPos[3]]);

        delete[] penguinPos;


        penguinPos = Tools::fastBitScan(board.oppos, &l);

        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
        moveFields = Tools::genMoveField(penguinPos[0], board.used) 
                | Tools::genMoveField(penguinPos[1], board.used) 
                | Tools::genMoveField(penguinPos[2], board.used)
                | Tools::genMoveField(penguinPos[3], board.used);

        movePoints -= Tools::popCount(moveFields & Globals::threes) * multpMovep3;
        movePoints -= Tools::popCount(moveFields & Globals::twos) * multpMovep2;
        movePoints -= Tools::popCount(moveFields & Globals::ones) * multpMovep1;

        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[0], board.used) & _fieldsAround[penguinPos[0]]);        
        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[1], board.used) & _fieldsAround[penguinPos[1]]);        
        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[2], board.used) & _fieldsAround[penguinPos[2]]);        
        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[3], board.used) & _fieldsAround[penguinPos[3]]);

        delete[] penguinPos;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 10:10
//        movePoints += Tools::popCount(RING2 & board.mypos);
//        movePoints += Tools::popCount(RING1 & board.mypos) * -1;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 1;         //Test 15:5Win ; 7:13Lost
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12Lost
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * -3;         //Test 10:10 
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 11:9Win
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 8:12 Lost
//        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 4:16Lost
//        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * -2;         //Test14:6Win ; 20:24Lost
//        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 84:76Win
//        movePoints += Tools::popCount(RING4 & board.mypos) * -2;
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 78:82Lost
//        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
        
        
//        movePoints += Tools::popCount(RING1 & board.mypos);         //Test Win 87:73
//        movePoints -= Tools::popCount(RING4 & board.mypos);
        
        
        
//        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12 Lost
//        movePoints += Tools::popCount(RING2 & board.mypos);
//        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
        
        

    #ifdef eval_reachPoints
        int reachablePoints = 0;

        u_int64_t reachableField = 0ULL;
        u_int64_t lastpos = board.mypos;
        while(lastpos){
            int l = 0;
            int *expandedPos = Tools::fastBitScan(lastpos, &l);
            u_int64_t movefield = 0ULL;
            for(int i = 0; i < l; ++i){
                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
            }
            delete[] expandedPos;
            lastpos = movefield;
            reachableField |= movefield;
        }

        reachablePoints += Tools::popCount(reachableField & Globals::threes) * 4;
        reachablePoints += Tools::popCount(reachableField & Globals::twos) * 2;
        reachablePoints += Tools::popCount(reachableField & Globals::ones);


        reachableField = 0ULL;
        lastpos = board.oppos;
        while(lastpos){
            int l = 0;
            int *expandedPos = Tools::fastBitScan(lastpos, &l);
            u_int64_t movefield = 0ULL;
            for(int i = 0; i < l; ++i){
                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
            }
            delete[] expandedPos;
            lastpos = movefield;
            reachableField |= movefield;
        }

        reachablePoints -= Tools::popCount(reachableField & Globals::threes) * 4;
        reachablePoints -= Tools::popCount(reachableField & Globals::twos) * 2;
        reachablePoints -= Tools::popCount(reachableField & Globals::ones);
    #endif

    #ifdef bigfield_eval
        int bigPoints = 0;

        u_int64_t bigmovefield = 0ULL;        
        int len;
        int* posis = Tools::bitScan(moveFields, &len);

        for(u_int8_t i = 0; i < len; i++)
            bigmovefield |= Tools::genMoveField(posis[i], board.used);

        bigPoints += Tools::popCount(bigmovefield & Globals::threes) * 4;
        bigPoints += Tools::popCount(bigmovefield & Globals::twos) * 2;
        bigPoints += Tools::popCount(bigmovefield & Globals::ones);

        delete[] posis;

        bigmovefield = 0ULL;        
        len = 0;
        posis = Tools::bitScan(moveFields, &len);

        for(u_int8_t i = 0; i < len; i++)
            bigmovefield |= Tools::genMoveField(posis[i], board.used);

        bigPoints -= Tools::popCount(bigmovefield & Globals::threes) * 4;
        bigPoints -= Tools::popCount(bigmovefield & Globals::twos) * 2;
        bigPoints -= Tools::popCount(bigmovefield & Globals::ones);

    //    for(int i = 0; i < l; i++)
    //    {
    //        if(Tools::popCount(_fieldsAround[penguinPos[i]] & Tools::getMoveField(penguinPos[i], board.used)) < 1)
    //            movePoints += 2;
    //    }

        delete[] posis;
    #endif

    //        std::cout << "Points: " << points << std::endl;

    //    Board qBoard = board;
    //    int qId = playerId;
    //    for(int i = 0; i < 1 && board.movecount < 60; ++i){
    //        int len;
    //        Move* moves = BoardTools::generatePossibleMoves(qBoard, qId, &len);
    //        BoardTools::apply(&qBoard, qId, moves[0]);
    //        qId != qId;
    //        delete[] moves;
    //    }
    //    
    //    int qpoints = qBoard.pointsdiff;
    //    if(qId != playerId){
    //        qpoints = -qpoints;
    //    }
    //    int qMovePoints = 0;
    //        
    //    if(qBoard.movecount < 60){
    //        int l = 0;
    //        int *penguinPos = Tools::fastBitScan(board.mypos, &l);
    //
    //        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
    //        u_int64_t moveFields = Tools::getMoveField(penguinPos[0], board.used) 
    //                | Tools::getMoveField(penguinPos[1], board.used) 
    //                | Tools::getMoveField(penguinPos[2], board.used)
    //                | Tools::getMoveField(penguinPos[3], board.used);
    //
    //
    //        qMovePoints += Tools::popCount(moveFields & Globals::threes) * 4;
    //        qMovePoints += Tools::popCount(moveFields & Globals::twos) * 2;
    //        qMovePoints += Tools::popCount(moveFields & Globals::ones);
    //
    //
    //        delete[] penguinPos;
    //
    //        penguinPos = Tools::fastBitScan(board.oppos, &l);
    //
    //        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
    //        moveFields = Tools::getMoveField(penguinPos[0], board.used) 
    //                | Tools::getMoveField(penguinPos[1], board.used) 
    //                | Tools::getMoveField(penguinPos[2], board.used)
    //                | Tools::getMoveField(penguinPos[3], board.used);
    //
    //        qMovePoints -= Tools::popCount(moveFields & Globals::threes) * 4;
    //        qMovePoints -= Tools::popCount(moveFields & Globals::twos) * 2;
    //        qMovePoints -= Tools::popCount(moveFields & Globals::ones);
    //
    //        delete[] penguinPos;
    //    }



    #ifdef bigfield_eval
        points *= 4;
        movePoints *= 2;
    #endif

        int result = points*multpPoints + movePoints*multpMovepo;
    #ifdef bigfield_eval
        result += bigPoints;
    #endif

    #ifdef eval_reachPoints
        result *= 4;
        result += reachablePoints;
    #endif
    #ifdef Q_SEARCH
        if(qsearch){
            Board qBoard = board;
            int qId = playerId;
            for(int i = 0; i < Q_SEARCH_DEEP && qBoard.movecount < 60; ++i){
                int l;
                Move* moves = BoardTools::generatePossibleMoves(qBoard, qId, &l);
                BoardTools::apply(&qBoard, qId, moves[0]);
                delete[] moves;
                qId = !qId;
            }
            int qPoints = evaluate(playerId, qBoard, false);
            result *= 2;
            result += qPoints;
        }
    #endif

        if(playerId != ID_WE){
            return -result;
        }
        return result;
    }
    
    int evaluateSetMoves(int playerId, Board board, bool qsearch)
    {
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
        u_int64_t moveFields = Tools::genMoveField(penguinPos[0], board.used) 
                | Tools::genMoveField(penguinPos[1], board.used) 
                | Tools::genMoveField(penguinPos[2], board.used)
                | Tools::genMoveField(penguinPos[3], board.used);
                
        int movePoints = 0;
//        movePoints += Tools::popCount((_fieldsAround[penguinPos[0]] & _fieldsAround[penguinPos[1]] & _fieldsAround[penguinPos[2]], _fieldsAround[penguinPos[3]]) & Globals::threes);
        movePoints += Tools::popCount(moveFields & Globals::threes) * 2;
        //movePoints += Tools::popCount(moveFields & Globals::twos) * 1;
        //movePoints += Tools::popCount(moveFields & Globals::ones);

        delete[] penguinPos;


        penguinPos = Tools::fastBitScan(board.oppos, &l);

        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
        moveFields = Tools::genMoveField(penguinPos[0], board.used) 
                | Tools::genMoveField(penguinPos[1], board.used) 
                | Tools::genMoveField(penguinPos[2], board.used)
                | Tools::genMoveField(penguinPos[3], board.used);

//        movePoints -= Tools::popCount((_fieldsAround[penguinPos[0]] & _fieldsAround[penguinPos[1]] & _fieldsAround[penguinPos[2]], _fieldsAround[penguinPos[3]]) & Globals::threes);
        movePoints -= Tools::popCount(moveFields & Globals::threes) * 2;
        //movePoints -= Tools::popCount(moveFields & Globals::twos) * 1;
        //movePoints -= Tools::popCount(moveFields & Globals::ones);

        delete[] penguinPos;
        
        int pluspoints = 0;
        
        if(board.movecount < 8)
        {
            if((Tools::popCount((board.mypos & Q1_all))) > 1)
                pluspoints -= 50;
            else
                pluspoints += 50;

            if((Tools::popCount((board.mypos & Q2_all))) > 1)
                pluspoints -= 50;
            else
                pluspoints += 50;

            if((Tools::popCount((board.mypos & Q3_all))) > 1)
                pluspoints -= 50;
            else
                pluspoints += 50;

            if((Tools::popCount((board.mypos & Q4_all))) > 1)
                pluspoints -= 50;
            else
                pluspoints += 50;
            
            /////////////////////////////
            
            if((Tools::popCount((board.oppos & Q1_best))) == 1)
                pluspoints += 25;
            else
                pluspoints -= 25;

            if((Tools::popCount((board.oppos & Q2_best))) == 1)
                pluspoints += 25;
            else
                pluspoints -= 25;

            if((Tools::popCount((board.oppos & Q3_best))) == 1)
                pluspoints += 25;
            else
                pluspoints -= 25;

            if((Tools::popCount((board.oppos & Q4_best))) == 1)
                pluspoints += 25;
            else
                pluspoints -= 25;
        }

        int result = points*multpPoints + movePoints*multpMovepo + pluspoints;

        if(playerId != ID_WE){
            return -result;
        }
        return result;
    }

}
