#include "navigation.h"
#include <iostream>
#include <sys/stat.h>

int alphaBetaTT(Board board, int depth, int alpha, int beta, int player, Move *resultMove, bool *timeIsUp){
    int value;
    
    if(depth == 0 || board.movecount >= 60){
        value = Evaluation::evaluate(player, board);
        
    
        struct timespec nowTime;
        clock_gettime(Globals::clockTime, &nowTime);

        *timeIsUp = (((
                (nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - 
                (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) > (TIME_FOR_CALC));
        
        return value;
    }
    int moveCount = 0;
    Move *moves = BoardTools::generatePossibleMoves(board, player, &moveCount);
    int best = MIN_AB_VALUE-1;
    int i;
    Board nextBoard;
    for(i = 0; i < moveCount; ++i){
        nextBoard = board;
        BoardTools::apply(&nextBoard, player, moves[i]);
        
        value = -alphaBetaTT(nextBoard, depth-1,-beta,-alpha, !player, 0, timeIsUp);

        if(value > best){
            best = value;
            
            if(resultMove) *resultMove = moves[i];
        }
        if(best > alpha)alpha = best;
        
        if(best >= beta){
            if(resultMove) *resultMove = moves[i];
            break;
        }
        if(*timeIsUp) break;
    }
    
    delete[] moves;
    return best;
}

int startDeep = START_DEEP;

int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove){
    bool timeIsUp = false;
        
    Move move;
    int d;
    if(startDeep > depth) d = depth;
    else if(board.movecount + startDeep > 60) d = 60 - board.movecount;
    else d = startDeep;
    
    for(; d <= depth && board.movecount + d <= 60; d++){

        firstguess = alphaBetaTT(board, d, -1000, 1000, ID_WE, &move, &timeIsUp);
        
        if(timeIsUp){
            break;
        }
        *resultMove = move;
    }
    startDeep = (d * 80) / 100;
    
    return firstguess;
}
