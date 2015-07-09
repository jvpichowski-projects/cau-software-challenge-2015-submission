#include "evaluation.h"
#include "boardstate.h"
typedef int PVOID;

namespace Evaluation
{

#define Q1_all 0x3C387878ULL
#define Q1_best 0x103000ULL

#define Q2_all 0x3C78787ULL
#define Q2_best 0x30300ULL

#define Q3_all 0xF0E1E1C0000000ULL
#define Q3_best 0x40C000000000ULL

#define Q4_all 0xF0F1E1E00000000ULL
#define Q4_best 0x60C0000000000ULL
    
#define _points 4
#define _moveFields 1
#define _ringFields 1
#define _rReachFields 1
#define _aReachFields 1
        
#define p3 3
#define p2 2
#define p1 1
    
    
    Evaluation::ToEvaluate evaluate;    
    Evaluation::ToEvaluate fastEvaluate;    
    
    int fastEval(int playerId, Board board){
        //you could improve the sort eval here
        int points = board.pointsdiff;
        
        //-------------------------------get positions--------------------------

        //load the penguin positions in arrays
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
        
        //define the unused fields around the penguins
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

        //count the number of unused fields
        ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);

        //count the value of the unused fields
        ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * p3;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * p2;
        ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * p1;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * p3;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * p2;
        ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * p1;
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        //valuate the points higher
        int result = points * _points
                    + _moveFields * (moveFieldCount + moveFieldPoints)
                    + _ringFields * (ringFieldCount + ringFieldPoints);
        
        if(playerId != ID_WE){
            //we need to invert the result if it is for the opponent
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
        
        //check if we have finished the set-phase
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
        
        int result = points * _points
                    + _moveFields * (moveFieldCount + moveFieldPoints )
                    + _ringFields * (ringFieldCount + ringFieldPoints );
        
        if(playerId != ID_WE) return -result;
        return result;
        
    }
    
    int evalIf(int playerId, Board board){
        
        int points = board.pointsdiff;

        if(board.movecount >= 60){
            if(playerId != ID_WE) return -points;
            return points;
        }
        
        //-------------------------------get positions--------------------------

        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //-------------------------------generate move fields-------------------
        
        //generate movefields for the next turn
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
        //count points of move fields
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
            
            //a reach field is the whole floe arround a penguin
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
            
            //mark the the moveFields as used for the board
            u_int64_t restrictedUsedForWe = board.used | moveFieldsOp;
            u_int64_t restrictedUsedForOp = board.used | moveFieldsWe;
            u_int64_t trash = 0;
            u_int64_t restrictedReachFieldWe = 0;
            u_int64_t restrictedReachFieldOp = 0;
            //get the floe which is restricted by the movefields of the opponent
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
        else{            
            //we are in the set-phase
            setMoveQuad = 200;
            
            //count how often we are in every quater.
            //if we are more than one time in a quater it is bad.
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
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        int result = points * _points
                    + _moveFields * (moveFieldCount + moveFieldPoints)                 //4 1
                    + _ringFields * (ringFieldCount + ringFieldPoints)                  //4 1
                    + _aReachFields * (totalReachFieldCount + totalReachFieldPoints)
                    + _rReachFields * (restrictedReachFieldCount + restrictedReachFieldPoints)
                    + setMoveQuad;
        
        if(playerId != ID_WE) return -result;
        return result;
        
    }
    
    int eval(int playerId, Board board){
        
        int points = board.pointsdiff;

        if(board.movecount >= 60){
            if(playerId != ID_WE) return -points;
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
        
        int result = points * _points
                    + _moveFields * (moveFieldCount + moveFieldPoints)                 //4 1
                    + _ringFields * (ringFieldCount + ringFieldPoints)                  //4 1
                    + _aReachFields * (totalReachFieldCount + totalReachFieldPoints)
                    + _rReachFields * (restrictedReachFieldCount + restrictedReachFieldPoints);
        
        if(playerId != ID_WE) return -result;
        
        return result;
        
    }
    
    int preEvaluate(int moveCount)
    {
        //this functions have an if statement to check if the set-phase is finished
        evaluate = &Evaluation::evalIf;
        fastEvaluate = &Evaluation::fastEvalIf;
        if(moveCount >= 8){
            //this functions could only be used if the set-phase is finished
            fastEvaluate = &Evaluation::fastEval;
            evaluate == &Evaluation::eval;
        }
        
        return 0; 
    }
}