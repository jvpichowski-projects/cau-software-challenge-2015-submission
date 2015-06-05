#include "navigation.h"
#include <iostream>
#include <sys/stat.h>

int evalCount;
int cutOff;
int nodesTraveled;
int totalEvalCount;
int totalCutOff;
int totalNodesTravled;


//int layer;
// method call using depth 5:
// minimax = alphaBetaTT(board, 5, -MATE, +MATE);
// TT stands for Transposition Table
int alphaBetaTT(Board board, int depth, int alpha, int beta, int player, Move *resultMove, bool *timeIsUp)
{
    ++nodesTraveled;
    
    int value;
    
    if(depth == 0 || board.movecount >= 60)
    {
        ++evalCount;
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
//    if(board.movecount == 8){
//        BoardTools::genMoveField(&board);
//    }
//    Board *boards = BoardTools::generatePossibleBoards(board, player, &moveCount);
    int best = MIN_AB_VALUE-1;
    int i;
    Board nextBoard;
#ifdef move_order_stats
    int bestMoveNum = 0;
#endif
    for(i = 0; i < moveCount; ++i)
    {        
        nextBoard = board;
        BoardTools::apply(&nextBoard, player, moves[i]);
        
        value = -alphaBetaTT(nextBoard, depth-1,-beta,-alpha, !player, 0, timeIsUp);

        //value = -alphaBetaTT(boards[i], depth-1,-beta,-alpha, !player, 0, timeIsUp);

        if(value > best){
            best = value;
#ifdef move_order_stats
            bestMoveNum = i;
#endif
            if(resultMove){
                //*resultMove = boards[i].lastMove;
                *resultMove = moves[i];
            }
        }
        if(best > alpha){
            alpha = best;
        }
        if(best >= beta){
            if(resultMove){
                //*resultMove = boards[i].lastMove;
                *resultMove = moves[i];
            }
            ++cutOff;
            break;
        }
        if(*timeIsUp){
            break;
        }
    }
#ifdef move_order_stats
    //not for set moves
    if(board.movecount >= 8){
       ++Globals::Log::move_order[moveCount][bestMoveNum];
    }
#endif
    
    delete[] moves;
    //delete[] boards;
    return best;
}

#ifdef FIXED_START_DEEP
int startDeep = START_DEEP;
#else
int startDeep = 1;
#endif

int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove)
{
    totalNodesTravled = 0;
    totalCutOff = 0;
    totalEvalCount = 0;
    
    bool timeIsUp = false;
        
    Move move;
    int d;
#if defined(FIXED_START_DEEP) || defined(DYN_START_DEEP) 
    if(startDeep > depth){
        d = depth;
    }else if(board.movecount + startDeep > 60){
        d = 60 - board.movecount;
    }else{
        d = startDeep;
    }
#endif
    for(; d <= depth && board.movecount + d <= 60; d++)
    {

        firstguess = alphaBetaTT(board, d, -1000, 1000, ID_WE, &move, &timeIsUp);
        
        std::cout << "Deep: " << d << " Nodes traveled: " << nodesTraveled << " Evals: " << evalCount << " CutOff: " << cutOff << " Value: " << firstguess << std::endl;
        totalNodesTravled += nodesTraveled;
        totalCutOff += cutOff;
        totalEvalCount += evalCount;
        nodesTraveled = 0;
        evalCount = 0;
        cutOff = 0;
        
        if(timeIsUp){
            break;
        }
        //only use move of finished searches
        *resultMove = move;
    }
#ifdef DYN_START_DEEP
    startDeep = (d * 80) / 100;
#endif
    std::cout << "ID-Bilance Deep: " << d-1 << " Nodes traveled: " << totalNodesTravled << " Evals: " << totalEvalCount << " CutOff: " << totalCutOff << " Value: " << firstguess << std::endl;
    Globals::Log::globalCutOff += totalCutOff;
    Globals::Log::globalEvalCount += totalEvalCount;
    Globals::Log::globalNodesTravled += totalNodesTravled;
    
    return firstguess;
}