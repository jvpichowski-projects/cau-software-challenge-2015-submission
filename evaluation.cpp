//Ver2

#include "evaluation.h"
#include "boardstate.h"
typedef int PVOID;

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
    
    
    int newEval(int playerId, Board board, bool qsearch){
        
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
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * 3;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * 2;
        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * 1;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * 3;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * 2;
        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * 1;
        
        
        int totalReachFieldCount = 0;
        int totalReachFieldPoints = 0;
        int restrictedReachFieldCount = 0;
        int restrictedReachFieldPoints = 0;
        int ringFieldCount = 0;
        int ringFieldPoints = 0;
        
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
            
            ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * 3;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * 2;
            ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * 1;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * 3;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * 2;
            ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * 1;
            
        //-------------------------------total reach field points---------------
            
            u_int64_t totalReachFieldWe = 0;
            u_int64_t totalReachFieldOp = 0;
            Tools::getReachableFields((~board.used) & ~FIT,
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &totalReachFieldWe, &totalReachFieldOp);
            
            totalReachFieldCount = Tools::popCount(totalReachFieldWe) - Tools::popCount(totalReachFieldOp);
            
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::threes)  * 3;
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::twos)    * 2;
            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::ones)    * 1;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::threes)  * 3;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::twos)    * 2;
            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::ones)    * 1;
            
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
            
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::threes)  * 3;
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::twos)    * 2;
            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::ones)    * 1;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::threes)  * 3;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::twos)    * 2;
            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::ones)    * 1;
            
        }
        
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
    
    int preEvaluate()
    {
        
        evaluate = &Evaluation::newEval;
        return 0; 
        
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
            //evaluate = &Evaluation::evaluateSetMoves;
            
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
            //evaluate = &Evaluation::evaluateNormal;
            
            multpPoints = 4;
            multpMovepo = 1;
            
            multpMovep1 = 0;
            multpMovep2 = 0;
            multpMovep3 = 2;
        }
    }
}
