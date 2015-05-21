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
        
        int result =  points * 4                                                //6
                    + moveFieldCount * 1 + moveFieldPoints * 1                  //4 1
                    + ringFieldCount * 1 + ringFieldPoints * 1                  //4 1
                    + totalReachFieldCount * 1 + totalReachFieldPoints * 1
                    + restrictedReachFieldCount * 1 + restrictedReachFieldPoints * 1;
        
        
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
        
        int b = 0;
        int *penguinPosWe = Tools::fastBitScan(board.mypos, &b);
        b = 0;
        int *penguinPosOp = Tools::fastBitScan(board.oppos, &b);
        
        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], board.used) 
                | Tools::genMoveField(penguinPosWe[1], board.used) 
                | Tools::genMoveField(penguinPosWe[2], board.used)
                | Tools::genMoveField(penguinPosWe[3], board.used);
        
        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], board.used) 
                | Tools::genMoveField(penguinPosOp[1], board.used) 
                | Tools::genMoveField(penguinPosOp[2], board.used)
                | Tools::genMoveField(penguinPosOp[3], board.used);
        
        int allLinePointsWe = 0;
        int allLinePointsOp = 0;
        allLinePointsWe += Tools::popCount(moveFieldsWe & Globals::threes) * Globals::Config::allLinePoints3;
        allLinePointsWe += Tools::popCount(moveFieldsWe & Globals::twos) * Globals::Config::allLinePoints2;
        allLinePointsWe += Tools::popCount(moveFieldsWe & Globals::ones) * Globals::Config::allLinePoints1;
        allLinePointsOp += Tools::popCount(moveFieldsOp & Globals::threes) * Globals::Config::allLinePoints3;
        allLinePointsOp += Tools::popCount(moveFieldsOp & Globals::twos) * Globals::Config::allLinePoints2;
        allLinePointsOp += Tools::popCount(moveFieldsOp & Globals::ones) * Globals::Config::allLinePoints1;
        
//        int singleLinePointsWe = 0;
//        int singleLinePointsOp = 0;
//        singleLinePointsWe += ((moveFieldsWe & Globals::threes) != 0) * Globals::Config::singleLinePoint3;
//        singleLinePointsWe += ((moveFieldsWe & Globals::twos) != 0) * Globals::Config::singleLinePoint2;
//        singleLinePointsWe += ((moveFieldsWe & Globals::ones) != 0) * Globals::Config::singleLinePoint1;
//        singleLinePointsOp += ((moveFieldsOp & Globals::threes) != 0) * Globals::Config::singleLinePoint3;
//        singleLinePointsOp += ((moveFieldsOp & Globals::twos) != 0) * Globals::Config::singleLinePoint2;
//        singleLinePointsOp += ((moveFieldsOp & Globals::ones) != 0) * Globals::Config::singleLinePoint1;
        
        
        u_int64_t reachFieldWe = 0;
        u_int64_t reachFieldOp = 0;     
        //a field resitricted through move lines of op
        u_int64_t restrictedReachFieldWe = 0;
        u_int64_t restrictedReachFieldOp = 0;
//        u_int64_t allRestrictedReachFieldWe = 0;
//        u_int64_t allRestrictedReachFieldOp = 0;
        if(board.movecount >= 8){
            Tools::getReachableFields(board.used, 
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &reachFieldWe, &reachFieldOp);
            
            u_int64_t unused = 0;
            u_int64_t used_with_linesOp = board.used & ~moveFieldsOp;
            
            Tools::getReachableFields(used_with_linesOp, 
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &restrictedReachFieldWe, &unused);
            
            unused = 0;
            u_int64_t used_with_linesWe = board.used & ~moveFieldsWe;
            
            Tools::getReachableFields(used_with_linesWe, 
                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
                    &unused, &restrictedReachFieldOp);
            
//            u_int64_t used_with_lines = used_with_linesOp & ~moveFieldsWe;
//            
//            Tools::getReachableFields(used_with_lines, 
//                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
//                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
//                    &allRestrictedReachFieldWe, &allRestrictedReachFieldOp);
                                    
        }
        
        int reachPointsWe = 0;
        int reachPointsOp = 0;
        reachPointsWe += Tools::popCount(reachFieldWe & Globals::threes) * Globals::Config::reachPoints3;
        reachPointsWe += Tools::popCount(reachFieldWe & Globals::twos) * Globals::Config::reachPoints2;
        reachPointsWe += Tools::popCount(reachFieldWe & Globals::ones) * Globals::Config::reachPoints1;
        reachPointsOp += Tools::popCount(reachFieldOp & Globals::threes) * Globals::Config::reachPoints3;
        reachPointsOp += Tools::popCount(reachFieldOp & Globals::twos) * Globals::Config::reachPoints2;
        reachPointsOp += Tools::popCount(reachFieldOp & Globals::ones) * Globals::Config::reachPoints1;
        
        int restrictedReachPointsWe = 0;
        int restrictedReachPointsOp = 0;
        restrictedReachPointsWe += Tools::popCount(restrictedReachFieldWe & Globals::threes) * Globals::Config::restrictedReachField3;
        restrictedReachPointsWe += Tools::popCount(restrictedReachFieldWe & Globals::twos) * Globals::Config::restrictedReachField2;
        restrictedReachPointsWe += Tools::popCount(restrictedReachFieldWe & Globals::ones) * Globals::Config::restrictedReachField1;
        restrictedReachPointsOp += Tools::popCount(restrictedReachFieldOp & Globals::threes) * Globals::Config::restrictedReachField3;
        restrictedReachPointsOp += Tools::popCount(restrictedReachFieldOp & Globals::twos) * Globals::Config::restrictedReachField2;
        restrictedReachPointsOp += Tools::popCount(restrictedReachFieldOp & Globals::ones) * Globals::Config::restrictedReachField1;
        
//        int allRestricedPoints = 0;
//        allRestricedPoints += Tools::popCount(allRestrictedReachFieldWe & Globals::threes) * Globals::Config::allRestrictedReachField3;
//        allRestricedPoints += Tools::popCount(allRestrictedReachFieldWe & Globals::twos) * Globals::Config::allRestrictedReachField2;
//        allRestricedPoints += Tools::popCount(allRestrictedReachFieldWe & Globals::ones) * Globals::Config::allRestrictedReachField1;
//        allRestricedPoints -= Tools::popCount(allRestrictedReachFieldOp & Globals::threes) * Globals::Config::allRestrictedReachField3;
//        allRestricedPoints -= Tools::popCount(allRestrictedReachFieldOp & Globals::twos) * Globals::Config::allRestrictedReachField2;
//        allRestricedPoints -= Tools::popCount(allRestrictedReachFieldOp & Globals::ones) * Globals::Config::allRestrictedReachField1;
                
        
        delete[] penguinPosWe;
        delete[] penguinPosOp;
        
        int reachPoints = 0;
        int allLinePoints = 0;
//        int singleLinePoints = 0;
        int restrictedReachPoints = 0;
        if(playerId != ID_WE){
            reachPoints -= reachPointsWe;
            reachPoints += reachPointsOp * 2;
            allLinePoints -= allLinePointsWe;
            allLinePoints += allLinePointsOp;
//            singleLinePoints -= singleLinePointsWe;
//            singleLinePoints += singleLinePointsOp;
            restrictedReachPoints -= restrictedReachPointsWe;
            restrictedReachPoints += restrictedReachPointsOp;
            
            return -points*Globals::Config::points+reachPoints+allLinePoints+restrictedReachPoints;
        }
        reachPoints += reachPointsWe;
        reachPoints -= reachPointsOp * 2;
        allLinePoints += allLinePointsWe;
        allLinePoints -= allLinePointsOp;
//        singleLinePoints += singleLinePointsWe;
//        singleLinePoints -= singleLinePointsOp;
        restrictedReachPoints += restrictedReachPointsWe;
        restrictedReachPoints -= restrictedReachPointsOp;
        
        return points*Globals::Config::points+reachPoints+allLinePoints+restrictedReachPoints;
        
        
//------------------------------------------------------------------------------
//        
//
//        int l = 0;
//        int *penguinPos = Tools::fastBitScan(board.mypos, &l);
//        
//        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
//        u_int64_t moveFields = Tools::genMoveField(penguinPos[0], board.used) 
//                | Tools::genMoveField(penguinPos[1], board.used) 
//                | Tools::genMoveField(penguinPos[2], board.used)
//                | Tools::genMoveField(penguinPos[3], board.used);
//                
//        int movePoints = 0;
//        movePoints += Tools::popCount(moveFields & Globals::threes) * multpMovep3;
//        movePoints += Tools::popCount(moveFields & Globals::twos) * multpMovep2;
//        movePoints += Tools::popCount(moveFields & Globals::ones) * multpMovep1;
//
//        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[0], board.used) & _fieldsAround[penguinPos[0]]);        
//        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[1], board.used) & _fieldsAround[penguinPos[1]]);        
//        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[2], board.used) & _fieldsAround[penguinPos[2]]);        
//        movePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[3], board.used) & _fieldsAround[penguinPos[3]]);
//
//        delete[] penguinPos;
//
//
//        penguinPos = Tools::fastBitScan(board.oppos, &l);
//
//        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
//        moveFields = Tools::genMoveField(penguinPos[0], board.used) 
//                | Tools::genMoveField(penguinPos[1], board.used) 
//                | Tools::genMoveField(penguinPos[2], board.used)
//                | Tools::genMoveField(penguinPos[3], board.used);
//
//        movePoints -= Tools::popCount(moveFields & Globals::threes) * multpMovep3;
//        movePoints -= Tools::popCount(moveFields & Globals::twos) * multpMovep2;
//        movePoints -= Tools::popCount(moveFields & Globals::ones) * multpMovep1;
//
//        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[0], board.used) & _fieldsAround[penguinPos[0]]);        
//        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[1], board.used) & _fieldsAround[penguinPos[1]]);        
//        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[2], board.used) & _fieldsAround[penguinPos[2]]);        
//        movePoints -= multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[3], board.used) & _fieldsAround[penguinPos[3]]);
//
//        delete[] penguinPos;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 10:10
////        movePoints += Tools::popCount(RING2 & board.mypos);
////        movePoints += Tools::popCount(RING1 & board.mypos) * -1;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 1;         //Test 15:5Win ; 7:13Lost
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12Lost
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * -3;         //Test 10:10 
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 11:9Win
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 8:12 Lost
////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 4:16Lost
////        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * -2;         //Test14:6Win ; 20:24Lost
////        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 84:76Win
////        movePoints += Tools::popCount(RING4 & board.mypos) * -2;
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 78:82Lost
////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
//        
//        
////        movePoints += Tools::popCount(RING1 & board.mypos);         //Test Win 87:73
////        movePoints -= Tools::popCount(RING4 & board.mypos);
//        
//        
//        
////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12 Lost
////        movePoints += Tools::popCount(RING2 & board.mypos);
////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
//        
//        
//
//    #ifdef eval_reachPoints
//        int reachablePoints = 0;
//
//        u_int64_t reachableField = 0ULL;
//        u_int64_t lastpos = board.mypos;
//        while(lastpos){
//            int l = 0;
//            int *expandedPos = Tools::fastBitScan(lastpos, &l);
//            u_int64_t movefield = 0ULL;
//            for(int i = 0; i < l; ++i){
//                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
//            }
//            delete[] expandedPos;
//            lastpos = movefield;
//            reachableField |= movefield;
//        }
//
//        reachablePoints += Tools::popCount(reachableField & Globals::threes) * 4;
//        reachablePoints += Tools::popCount(reachableField & Globals::twos) * 2;
//        reachablePoints += Tools::popCount(reachableField & Globals::ones);
//
//
//        reachableField = 0ULL;
//        lastpos = board.oppos;
//        while(lastpos){
//            int l = 0;
//            int *expandedPos = Tools::fastBitScan(lastpos, &l);
//            u_int64_t movefield = 0ULL;
//            for(int i = 0; i < l; ++i){
//                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
//            }
//            delete[] expandedPos;
//            lastpos = movefield;
//            reachableField |= movefield;
//        }
//
//        reachablePoints -= Tools::popCount(reachableField & Globals::threes) * 4;
//        reachablePoints -= Tools::popCount(reachableField & Globals::twos) * 2;
//        reachablePoints -= Tools::popCount(reachableField & Globals::ones);
//    #endif
//
//    #ifdef bigfield_eval
//        int bigPoints = 0;
//
//        u_int64_t bigmovefield = 0ULL;        
//        int len;
//        int* posis = Tools::bitScan(moveFields, &len);
//
//        for(u_int8_t i = 0; i < len; i++)
//            bigmovefield |= Tools::genMoveField(posis[i], board.used);
//
//        bigPoints += Tools::popCount(bigmovefield & Globals::threes) * 4;
//        bigPoints += Tools::popCount(bigmovefield & Globals::twos) * 2;
//        bigPoints += Tools::popCount(bigmovefield & Globals::ones);
//
//        delete[] posis;
//
//        bigmovefield = 0ULL;        
//        len = 0;
//        posis = Tools::bitScan(moveFields, &len);
//
//        for(u_int8_t i = 0; i < len; i++)
//            bigmovefield |= Tools::genMoveField(posis[i], board.used);
//
//        bigPoints -= Tools::popCount(bigmovefield & Globals::threes) * 4;
//        bigPoints -= Tools::popCount(bigmovefield & Globals::twos) * 2;
//        bigPoints -= Tools::popCount(bigmovefield & Globals::ones);
//
//    //    for(int i = 0; i < l; i++)
//    //    {
//    //        if(Tools::popCount(_fieldsAround[penguinPos[i]] & Tools::getMoveField(penguinPos[i], board.used)) < 1)
//    //            movePoints += 2;
//    //    }
//
//        delete[] posis;
//    #endif
//
//    //        std::cout << "Points: " << points << std::endl;
//
//    //    Board qBoard = board;
//    //    int qId = playerId;
//    //    for(int i = 0; i < 1 && board.movecount < 60; ++i){
//    //        int len;
//    //        Move* moves = BoardTools::generatePossibleMoves(qBoard, qId, &len);
//    //        BoardTools::apply(&qBoard, qId, moves[0]);
//    //        qId != qId;
//    //        delete[] moves;
//    //    }
//    //    
//    //    int qpoints = qBoard.pointsdiff;
//    //    if(qId != playerId){
//    //        qpoints = -qpoints;
//    //    }
//    //    int qMovePoints = 0;
//    //        
//    //    if(qBoard.movecount < 60){
////        int l = 0;
////        int *penguinPos = Tools::fastBitScan(board.mypos, &l);
////        
////        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
////        u_int64_t moveFields = Tools::genMoveField(penguinPos[0], board.used) 
////                | Tools::genMoveField(penguinPos[1], board.used) 
////                | Tools::genMoveField(penguinPos[2], board.used)
////                | Tools::genMoveField(penguinPos[3], board.used);
////                
////
////        allLinePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[0], board.used) & _fieldsAround[penguinPos[0]]);        
////        allLinePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[1], board.used) & _fieldsAround[penguinPos[1]]);        
////        allLinePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[2], board.used) & _fieldsAround[penguinPos[2]]);        
////        allLinePoints += multpFiAr*Tools::popCount(Tools::genMoveField(penguinPos[3], board.used) & _fieldsAround[penguinPos[3]]);
////
//    //
////        delete[] penguinPos;
////
////        penguinPos = Tools::fastBitScan(board.oppos, &l);
////
////        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
////        moveFields = Tools::genMoveField(penguinPos[0], board.used) 
////                | Tools::genMoveField(penguinPos[1], board.used) 
////                | Tools::genMoveField(penguinPos[2], board.used)
////                | Tools::genMoveField(penguinPos[3], board.used);
////
////        allLinePoints -= Tools::popCount(moveFields & Globals::threes) * multpMovep3;
////        allLinePoints -= Tools::popCount(moveFields & Globals::twos) * multpMovep2;
////        allLinePoints -= Tools::popCount(moveFields & Globals::ones) * multpMovep1;
////
////        delete[] penguinPos;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 10:10
//////        movePoints += Tools::popCount(RING2 & board.mypos);
//////        movePoints += Tools::popCount(RING1 & board.mypos) * -1;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 1;         //Test 15:5Win ; 7:13Lost
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12Lost
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * -3;         //Test 10:10 
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 11:9Win
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 8:12 Lost
//////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 4:16Lost
//////        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * -2;         //Test14:6Win ; 20:24Lost
//////        movePoints += Tools::popCount(RING4 & board.mypos) * 2;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 2;         //Test 84:76Win
//////        movePoints += Tools::popCount(RING4 & board.mypos) * -2;
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 78:82Lost
//////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
////        
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos);         //Test Win 87:73
//////        movePoints -= Tools::popCount(RING4 & board.mypos);
////        
////        
////        
//////        movePoints += Tools::popCount(RING1 & board.mypos) * 3;         //Test 8:12 Lost
//////        movePoints += Tools::popCount(RING2 & board.mypos);
//////        movePoints += Tools::popCount(RING4 & board.mypos) * -1;
////        
////        
////
////    #ifdef eval_reachPoints
////        int reachablePoints = 0;
////
////        u_int64_t reachableField = 0ULL;
////        u_int64_t lastpos = board.mypos;
////        while(lastpos){
////            int l = 0;
////            int *expandedPos = Tools::fastBitScan(lastpos, &l);
////            u_int64_t movefield = 0ULL;
////            for(int i = 0; i < l; ++i){
////                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
////            }
////            delete[] expandedPos;
////            lastpos = movefield;
////            reachableField |= movefield;
////        }
////
////        reachablePoints += Tools::popCount(reachableField & Globals::threes) * 4;
////        reachablePoints += Tools::popCount(reachableField & Globals::twos) * 2;
////        reachablePoints += Tools::popCount(reachableField & Globals::ones);
////
////
////        reachableField = 0ULL;
////        lastpos = board.oppos;
////        while(lastpos){
////            int l = 0;
////            int *expandedPos = Tools::fastBitScan(lastpos, &l);
////            u_int64_t movefield = 0ULL;
////            for(int i = 0; i < l; ++i){
////                movefield |= Tools::genMoveField(expandedPos[i], reachableField | board.used); 
////            }
////            delete[] expandedPos;
////            lastpos = movefield;
////            reachableField |= movefield;
////        }
////
////        reachablePoints -= Tools::popCount(reachableField & Globals::threes) * 4;
////        reachablePoints -= Tools::popCount(reachableField & Globals::twos) * 2;
////        reachablePoints -= Tools::popCount(reachableField & Globals::ones);
////    #endif
////
////    #ifdef bigfield_eval
////        int bigPoints = 0;
////
////        u_int64_t bigmovefield = 0ULL;        
////        int len;
////        int* posis = Tools::bitScan(moveFields, &len);
////
////        for(u_int8_t i = 0; i < len; i++)
////            bigmovefield |= Tools::genMoveField(posis[i], board.used);
////
////        bigPoints += Tools::popCount(bigmovefield & Globals::threes) * 4;
////        bigPoints += Tools::popCount(bigmovefield & Globals::twos) * 2;
////        bigPoints += Tools::popCount(bigmovefield & Globals::ones);
////
////        delete[] posis;
////
////        bigmovefield = 0ULL;        
////        len = 0;
////        posis = Tools::bitScan(moveFields, &len);
////
////        for(u_int8_t i = 0; i < len; i++)
////            bigmovefield |= Tools::genMoveField(posis[i], board.used);
////
////        bigPoints -= Tools::popCount(bigmovefield & Globals::threes) * 4;
////        bigPoints -= Tools::popCount(bigmovefield & Globals::twos) * 2;
////        bigPoints -= Tools::popCount(bigmovefield & Globals::ones);
////
////    //    for(int i = 0; i < l; i++)
////    //    {
////    //        if(Tools::popCount(_fieldsAround[penguinPos[i]] & Tools::getMoveField(penguinPos[i], board.used)) < 1)
////    //            movePoints += 2;
////    //    }
////
////        delete[] posis;
////    #endif
////
////    //        std::cout << "Points: " << points << std::endl;
////
////    //    Board qBoard = board;
////    //    int qId = playerId;
////    //    for(int i = 0; i < 1 && board.movecount < 60; ++i){
////    //        int len;
////    //        Move* moves = BoardTools::generatePossibleMoves(qBoard, qId, &len);
////    //        BoardTools::apply(&qBoard, qId, moves[0]);
////    //        qId != qId;
////    //        delete[] moves;
////    //    }
////    //    
////    //    int qpoints = qBoard.pointsdiff;
////    //    if(qId != playerId){
////    //        qpoints = -qpoints;
////    //    }
////    //    int qMovePoints = 0;
////    //        
////    //    if(qBoard.movecount < 60){
////    //        int l = 0;
////    //        int *penguinPos = Tools::fastBitScan(board.mypos, &l);
////    //
////    //        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
////    //        u_int64_t moveFields = Tools::getMoveField(penguinPos[0], board.used) 
////    //                | Tools::getMoveField(penguinPos[1], board.used) 
////    //                | Tools::getMoveField(penguinPos[2], board.used)
////    //                | Tools::getMoveField(penguinPos[3], board.used);
////    //
////    //
////    //        qMovePoints += Tools::popCount(moveFields & Globals::threes) * 4;
////    //        qMovePoints += Tools::popCount(moveFields & Globals::twos) * 2;
////    //        qMovePoints += Tools::popCount(moveFields & Globals::ones);
////    //
////    //
////    //        delete[] penguinPos;
////    //
////    //        penguinPos = Tools::fastBitScan(board.oppos, &l);
////    //
////    //        //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
////    //        moveFields = Tools::getMoveField(penguinPos[0], board.used) 
////    //                | Tools::getMoveField(penguinPos[1], board.used) 
////    //                | Tools::getMoveField(penguinPos[2], board.used)
////    //                | Tools::getMoveField(penguinPos[3], board.used);
////    //
////    //        qMovePoints -= Tools::popCount(moveFields & Globals::threes) * 4;
////    //        qMovePoints -= Tools::popCount(moveFields & Globals::twos) * 2;
////    //        qMovePoints -= Tools::popCount(moveFields & Globals::ones);
////    //
////    //        delete[] penguinPos;
////    //    }
////
////
////
////    #ifdef bigfield_eval
////        points *= 4;
////        allLinePoints *= 2;
////    #endif
////
////        int result = points*multpPoints + allLinePoints*multpMovepo;
////    #ifdef bigfield_eval
////        result += bigPoints;
////    #endif
////
////    #ifdef eval_reachPoints
////        result *= 4;
////        result += reachablePoints;
////    #endif
////    #ifdef Q_SEARCH
////        if(qsearch){
////            Board qBoard = board;
////            int qId = playerId;
////            for(int i = 0; i < Q_SEARCH_DEEP && qBoard.movecount < 60; ++i){
////                int l;
////                Move* moves = BoardTools::generatePossibleMoves(qBoard, qId, &l);
////                BoardTools::apply(&qBoard, qId, moves[0]);
////                delete[] moves;
////                qId = !qId;
////            }
////            int qPoints = evaluate(playerId, qBoard, false);
////            result *= 2;
////            result += qPoints;
////        }
////    #endif
////
////        if(playerId != ID_WE){
////            return -result;
////        }
////        return result;
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
