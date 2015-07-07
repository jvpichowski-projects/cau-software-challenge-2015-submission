#include "navigation.h"
#include <iostream>
#include <sys/stat.h>

/**
 * Standart alphabeta
 * 
 * @param board the current board
 * @param depth the maximal search depth
 * @param alpha 
 * @param beta
 * @param player the current player
 * @param resultMove 0 if not first iteration otherwise a pointer to the resulting move
 * @param timeIsUp 
 * @return 
 */
int alphabeta(Board board, int depth, int alpha, int beta, int player, Move *resultMove, bool *timeIsUp){
    int value;
    
    if(depth == 0 || board.movecount >= 60){
        //evaluate state if reached max depth or max move count
        value = Evaluation::evaluate(player, board);
        
        //fetch time
        struct timespec nowTime;
        clock_gettime(Globals::clockTime, &nowTime);

        //check if time is up
        *timeIsUp = (((
                (nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - 
                (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) > (TIME_FOR_CALC));
        
        return value;
    }
    //generate move list
    int moveCount = 0;
    Move *moves = BoardTools::generatePossibleMoves(board, player, &moveCount);
    int best = MIN_AB_VALUE-1;
    int i;
    Board nextBoard;
    //test each move
    for(i = 0; i < moveCount; ++i){
        //copy state
        nextBoard = board;
        //apply move to board
        BoardTools::apply(&nextBoard, player, moves[i]);
        
        value = -alphabeta(nextBoard, depth-1,-beta,-alpha, !player, 0, timeIsUp);

        if(value > best){
            best = value;
            //if first iteration change resulting move to new best
            if(resultMove) *resultMove = moves[i];
        }
        if(best > alpha)alpha = best;
        
        if(best >= beta){
            //cutoff
            //if first iteration change resulting move to new best
            if(resultMove) *resultMove = moves[i];
            break;
        }
        //return immediately from loop if time is up
        if(*timeIsUp) break;
    }
    
    delete[] moves;
    return best;
}

//default start deep
int startDeep = START_DEEP;

int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove){
    bool timeIsUp = false;
        
    Move move;
    int d;
    //only search to max depth
    if(startDeep > depth) d = depth;
    //only search up to 60 moves (max depth of the game)
    else if(board.movecount + startDeep > 60) d = 60 - board.movecount;
    //start by default with the given start deep
    else d = startDeep;
    
    for(; d <= depth && board.movecount + d <= 60; d++){

        firstguess = alphabeta(board, d, -1000, 1000, ID_WE, &move, &timeIsUp);
        
        if(timeIsUp){
            break;
        }
        //only use move if search was finished before time up
        *resultMove = move;
    }
    //raise the start deep by 80 percent
    startDeep = (d * 80) / 100;
    
    //return the alphabeta value for the result move
    return firstguess;
}
