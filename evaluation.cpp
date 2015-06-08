//Ver2

#include "evaluation.h"
#include "boardstate.h"
typedef int PVOID;

namespace Evaluation
{
#define jonas
    
#define p3 3 //4 //7
#define p2 2 //2 //3
#define p1 1 //1 //1

//#define p 1 //1 //2 //1 //3 //1 //1
//#define c 1 //1 //1 //2 //1 //3 //0
    
    
    Evaluation::ToEvaluate evaluate;    
    Evaluation::ToEvaluate fastEvaluate;    
    
    int fastEval(int playerId, Board board){
        //you could improve the sort eval here
        int points = board.pointsdiff;
        
        //-------------------------------get positions--------------------------

        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //-------------------------------generate move fields-------------------
        
        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], board.used) 
                | Tools::genMoveField(penguinPosWe[1], board.used) 
                | Tools::genMoveField(penguinPosWe[2], board.used)
                | Tools::genMoveField(penguinPosWe[3], board.used);
        
        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], board.used) 
                | Tools::genMoveField(penguinPosOp[1], board.used) 
                | Tools::genMoveField(penguinPosOp[2], board.used)
                | Tools::genMoveField(penguinPosOp[3], board.used);
        
        //count number of move fields
        int moveFieldCount = Tools::popCount(moveFieldsWe) - Tools::popCount(moveFieldsOp);
        int moveFieldPoints = 0;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * p3;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * p2;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * p1;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * p3;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * p2;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * p1;
        
        
        int ringFieldCount = 0;
        int ringFieldPoints = 0;
            
        //-------------------------------ring points----------------------------
            
        u_int64_t ringsWe = _fieldsAround[penguinPosWe[0]] | 
                _fieldsAround[penguinPosWe[1]] | 
                _fieldsAround[penguinPosWe[2]] | 
                _fieldsAround[penguinPosWe[3]];
        ringsWe &= ~board.used;

        u_int64_t ringsOp = _fieldsAround[penguinPosOp[0]] | 
                _fieldsAround[penguinPosOp[1]] | 
                _fieldsAround[penguinPosOp[2]] | 
                _fieldsAround[penguinPosOp[3]];
        ringsOp &= ~board.used;

        ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);

        ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * p3;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * p2;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * p1;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * p3;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * p2;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * p1;
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        int result = points * Globals::Config::points
                    + Globals::Config::moveFields * (moveFieldCount + moveFieldPoints)
                    + Globals::Config::ringFields * (ringFieldCount + ringFieldPoints);
        
        if(playerId != ID_WE){
            return -result;
        }
        return result;
        
    }
    
    int fastEvalIf(int playerId, Board board){
        //you could improve the sort eval here
        int points = board.pointsdiff;
        
        //-------------------------------get positions--------------------------

        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //-------------------------------generate move fields-------------------
        
        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], board.used) 
                | Tools::genMoveField(penguinPosWe[1], board.used) 
                | Tools::genMoveField(penguinPosWe[2], board.used)
                | Tools::genMoveField(penguinPosWe[3], board.used);
        
        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], board.used) 
                | Tools::genMoveField(penguinPosOp[1], board.used) 
                | Tools::genMoveField(penguinPosOp[2], board.used)
                | Tools::genMoveField(penguinPosOp[3], board.used);
        
        //count number of move fields
        int moveFieldCount = Tools::popCount(moveFieldsWe) - Tools::popCount(moveFieldsOp);
        int moveFieldPoints = 0;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * p3;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * p2;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * p1;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * p3;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * p2;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * p1;
        
        
        int ringFieldCount = 0;
        int ringFieldPoints = 0;
        int setMoveQuad = 0;
        
        //why not >=????
        if(board.movecount >= 8){
            
        //-------------------------------ring points----------------------------
            
            u_int64_t ringsWe = _fieldsAround[penguinPosWe[0]] | 
                    _fieldsAround[penguinPosWe[1]] | 
                    _fieldsAround[penguinPosWe[2]] | 
                    _fieldsAround[penguinPosWe[3]];
            ringsWe &= ~board.used;
            
            u_int64_t ringsOp = _fieldsAround[penguinPosOp[0]] | 
                    _fieldsAround[penguinPosOp[1]] | 
                    _fieldsAround[penguinPosOp[2]] | 
                    _fieldsAround[penguinPosOp[3]];
            ringsOp &= ~board.used;
            
            ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);
            
            ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * p3;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * p2;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * p1;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * p3;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * p2;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * p1;
            
        }
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        int result = points * Globals::Config::points
                    + Globals::Config::moveFields * (moveFieldCount + moveFieldPoints )
                    + Globals::Config::ringFields * (ringFieldCount + ringFieldPoints )
                    + setMoveQuad;
        
        if(playerId != ID_WE){
            return -result;
        }
        return result;
        
    }
    
    int newEvalIf(int playerId, Board board){
        
        int points = board.pointsdiff;

        if(board.movecount >= 60){
            if(playerId != ID_WE){
                return -points;
            }
            return points;
        }
        
        //-------------------------------get positions--------------------------

        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //-------------------------------generate move fields-------------------
        
        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], board.used) 
                | Tools::genMoveField(penguinPosWe[1], board.used) 
                | Tools::genMoveField(penguinPosWe[2], board.used)
                | Tools::genMoveField(penguinPosWe[3], board.used);
        
        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], board.used) 
                | Tools::genMoveField(penguinPosOp[1], board.used) 
                | Tools::genMoveField(penguinPosOp[2], board.used)
                | Tools::genMoveField(penguinPosOp[3], board.used);
        
        //count number of move fields
        int moveFieldCount = Tools::popCount(moveFieldsWe) - Tools::popCount(moveFieldsOp);
        int moveFieldPoints = 0;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * p3;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * p2;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * p1;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * p3;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * p2;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * p1;
        
        
        int totalReachFieldCount = 0;
        int totalReachFieldPoints = 0;
        int restrictedReachFieldCount = 0;
        int restrictedReachFieldPoints = 0;
        int ringFieldCount = 0;
        int ringFieldPoints = 0;
#ifdef jonas
        int setMoveQuad = 0;
#endif
        //why not >=????
        if(board.movecount >= 8){
            
        //-------------------------------ring points----------------------------
            
            u_int64_t ringsWe = _fieldsAround[penguinPosWe[0]] | 
                    _fieldsAround[penguinPosWe[1]] | 
                    _fieldsAround[penguinPosWe[2]] | 
                    _fieldsAround[penguinPosWe[3]];
            ringsWe &= ~board.used;
            
            u_int64_t ringsOp = _fieldsAround[penguinPosOp[0]] | 
                    _fieldsAround[penguinPosOp[1]] | 
                    _fieldsAround[penguinPosOp[2]] | 
                    _fieldsAround[penguinPosOp[3]];
            ringsOp &= ~board.used;
            
            ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);
            
            ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * p3;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * p2;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * p1;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * p3;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * p2;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * p1;
            
        //-------------------------------total reach field points---------------
            
            u_int64_t totalReachFieldWe = 0;
            u_int64_t totalReachFieldOp = 0;
            Tools::getReachableFields((~board.used) & ~FIT,
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &totalReachFieldWe, &totalReachFieldOp);
            
            totalReachFieldCount = Tools::popCount(totalReachFieldWe) - Tools::popCount(totalReachFieldOp);
            
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::threes)  * p3;
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::twos)    * p2;
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::ones)    * p1;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::threes)  * p3;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::twos)    * p2;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::ones)    * p1;
            
        //-------------------------------restricted reach field points----------
            
            u_int64_t restrictedUsedForWe = board.used | moveFieldsOp;
            u_int64_t restrictedUsedForOp = board.used | moveFieldsWe;
            u_int64_t trash = 0;
            u_int64_t restrictedReachFieldWe = 0;
            u_int64_t restrictedReachFieldOp = 0;
            Tools::getReachableFields((~restrictedUsedForWe) & ~FIT,            //TODO remove because even if it is set it is not counted in popcount with &
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &restrictedReachFieldWe, &trash);
            
            trash = 0;
            Tools::getReachableFields((~restrictedUsedForOp) & ~FIT,
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &trash, &restrictedReachFieldOp);
            
            restrictedReachFieldCount = Tools::popCount(restrictedReachFieldWe) - Tools::popCount(restrictedReachFieldOp);
            
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::threes)  * p3;
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::twos)    * p2;
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::ones)    * p1;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::threes)  * p3;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::twos)    * p2;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::ones)    * p1;
            
        }
#ifdef jonas
        else{
            setMoveQuad = 200;
            
            if((Tools::fastPopCount((board.mypos & Q1_all))) > 1)
                setMoveQuad -= 100;
            if((Tools::fastPopCount((board.mypos & Q2_all))) > 1)
                setMoveQuad -= 100;
            if((Tools::fastPopCount((board.mypos & Q3_all))) > 1)
                setMoveQuad -= 100;
            if((Tools::fastPopCount((board.mypos & Q4_all))) > 1)
                setMoveQuad -= 100;

            if((Tools::fastPopCount((board.mypos & Q1_best))) == 1)
                setMoveQuad += 50;
            if((Tools::fastPopCount((board.mypos & Q2_best))) == 1)
                setMoveQuad += 50;
            if((Tools::fastPopCount((board.mypos & Q3_best))) == 1)
                setMoveQuad += 50;
            if((Tools::fastPopCount((board.mypos & Q4_best))) == 1)
                setMoveQuad += 50;
        }
#endif
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        //641410000
        
//        int result =  points * 4                                                //6
//                    + moveFieldCount * 1 + moveFieldPoints * 1                  //4 1
//                    + ringFieldCount * 0 + ringFieldPoints * 0                  //4 1
//                    + totalReachFieldCount * 1 + totalReachFieldPoints * 1
//                    + restrictedReachFieldCount * 1 + restrictedReachFieldPoints * 1;
        int result = points * Globals::Config::points
                    + Globals::Config::moveFields * (moveFieldCount + moveFieldPoints)                 //4 1
                    + Globals::Config::ringFields * (ringFieldCount + ringFieldPoints)                  //4 1
                    + Globals::Config::aReachFields * (totalReachFieldCount + totalReachFieldPoints)
                    + Globals::Config::rReachFields * (restrictedReachFieldCount + restrictedReachFieldPoints)
#ifdef jonas
        + setMoveQuad
#endif 
        ;
        
        if(playerId != ID_WE){
            return -result;
        }
        return result;
        
    }
    
    int newEval(int playerId, Board board){
        
        int points = board.pointsdiff;

        if(board.movecount >= 60){
            if(playerId != ID_WE){
                return -points;
            }
            return points;
        }
        
        //-------------------------------get positions--------------------------

        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //-------------------------------generate move fields-------------------
        
        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], board.used) 
                | Tools::genMoveField(penguinPosWe[1], board.used) 
                | Tools::genMoveField(penguinPosWe[2], board.used)
                | Tools::genMoveField(penguinPosWe[3], board.used);
        
        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], board.used) 
                | Tools::genMoveField(penguinPosOp[1], board.used) 
                | Tools::genMoveField(penguinPosOp[2], board.used)
                | Tools::genMoveField(penguinPosOp[3], board.used);
        
        //count number of move fields
        int moveFieldCount = Tools::popCount(moveFieldsWe) - Tools::popCount(moveFieldsOp);
        int moveFieldPoints = 0;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * p3;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * p2;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * p1;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * p3;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * p2;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * p1;
        
        
        int totalReachFieldCount = 0;
        int totalReachFieldPoints = 0;
        int restrictedReachFieldCount = 0;
        int restrictedReachFieldPoints = 0;
        int ringFieldCount = 0;
        int ringFieldPoints = 0;
        int setMoveQuad = 0;
        
            
        //-------------------------------ring points----------------------------
            
        u_int64_t ringsWe = _fieldsAround[penguinPosWe[0]] | 
                _fieldsAround[penguinPosWe[1]] | 
                _fieldsAround[penguinPosWe[2]] | 
                _fieldsAround[penguinPosWe[3]];
        ringsWe &= ~board.used;

        u_int64_t ringsOp = _fieldsAround[penguinPosOp[0]] | 
                _fieldsAround[penguinPosOp[1]] | 
                _fieldsAround[penguinPosOp[2]] | 
                _fieldsAround[penguinPosOp[3]];
        ringsOp &= ~board.used;

        ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);

        ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * p3;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * p2;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * p1;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * p3;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * p2;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * p1;

    //-------------------------------total reach field points---------------

        u_int64_t totalReachFieldWe = 0;
        u_int64_t totalReachFieldOp = 0;
        Tools::getReachableFields((~board.used) & ~FIT,
                penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                &totalReachFieldWe, &totalReachFieldOp);

        totalReachFieldCount = Tools::popCount(totalReachFieldWe) - Tools::popCount(totalReachFieldOp);

        totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::threes)  * p3;
        totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::twos)    * p2;
        totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::ones)    * p1;
        totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::threes)  * p3;
        totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::twos)    * p2;
        totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::ones)    * p1;

    //-------------------------------restricted reach field points----------

        u_int64_t restrictedUsedForWe = board.used | moveFieldsOp;
        u_int64_t restrictedUsedForOp = board.used | moveFieldsWe;
        u_int64_t trash = 0;
        u_int64_t restrictedReachFieldWe = 0;
        u_int64_t restrictedReachFieldOp = 0;
        Tools::getReachableFields((~restrictedUsedForWe) & ~FIT,            //TODO remove because even if it is set it is not counted in popcount with &
                penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                &restrictedReachFieldWe, &trash);

        trash = 0;
        Tools::getReachableFields((~restrictedUsedForOp) & ~FIT,
                penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                &trash, &restrictedReachFieldOp);

        restrictedReachFieldCount = Tools::popCount(restrictedReachFieldWe) - Tools::popCount(restrictedReachFieldOp);

        restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::threes)  * p3;
        restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::twos)    * p2;
        restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::ones)    * p1;
        restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::threes)  * p3;
        restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::twos)    * p2;
        restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::ones)    * p1;

       
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        //641410000
        
//        int result =  points * 4                                                //6
//                    + moveFieldCount * 1 + moveFieldPoints * 1                  //4 1
//                    + ringFieldCount * 0 + ringFieldPoints * 0                  //4 1
//                    + totalReachFieldCount * 1 + totalReachFieldPoints * 1
//                    + restrictedReachFieldCount * 1 + restrictedReachFieldPoints * 1;
        int result = points * Globals::Config::points
                    + Globals::Config::moveFields * (moveFieldCount + moveFieldPoints)                 //4 1
                    + Globals::Config::ringFields * (ringFieldCount + ringFieldPoints)                  //4 1
                    + Globals::Config::aReachFields * (totalReachFieldCount + totalReachFieldPoints)
                    + Globals::Config::rReachFields * (restrictedReachFieldCount + restrictedReachFieldPoints);
        
        if(playerId != ID_WE){
            return -result;
        }
        return result;
        
    }
    
    int preEvaluate(int moveCount)
    {
        
        //add your pre eval code here!
        //use either setEval or newEval 
        evaluate = &Evaluation::newEvalIf;
        fastEvaluate = &Evaluation::fastEvalIf;
        if(moveCount >= 8){
            fastEvaluate = &Evaluation::fastEval;
            evaluate == &Evaluation::newEval;
        }
        
        //you could change this params in Globals::Config
//        points;
//        moveFields;
//        ringFields;
//        rReachFields;
//        aReachFields;
        
        
        return 0; 
    }
}
