#include "evaluation.h"

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
    movePoints += Tools::popCount(moveFields & Globals::threes) * 4;
    movePoints += Tools::popCount(moveFields & Globals::twos) * 2;
    movePoints += Tools::popCount(moveFields & Globals::ones);


    delete[] penguinPos;

    penguinPos = Tools::fastBitScan(board.oppos, &l);

    //wenn sich zwei linien kreuzen wird das kreuz-feld nur einmal gerechnet
    moveFields = Tools::getMoveField(penguinPos[0], board.used) 
            | Tools::getMoveField(penguinPos[1], board.used) 
            | Tools::getMoveField(penguinPos[2], board.used)
            | Tools::getMoveField(penguinPos[3], board.used);

    movePoints -= Tools::popCount(moveFields & Globals::threes) * 4;
    movePoints -= Tools::popCount(moveFields & Globals::twos) * 2;
    movePoints -= Tools::popCount(moveFields & Globals::ones);


    delete[] penguinPos;

//        std::cout << "Points: " << points << std::endl;
       

    int result = points + movePoints;
        
        
    if(playerId != ID_WE){
        return -result;
    }
    return result;
}
