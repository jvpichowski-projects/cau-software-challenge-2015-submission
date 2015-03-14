/*
 * Copyright (c) 2009-2010, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Oracle nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

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
        value = evaluate(player, board, true);
        
    
        struct timespec nowTime;
        clock_gettime(Globals::clockTime, &nowTime);

        if((((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) > (TIME_FOR_CALC)){
            *timeIsUp = true;
        }
        
        return value;
    }
    int moveCount;
    Move *moves = BoardTools::generatePossibleMoves(board, player, &moveCount);
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
        
#ifdef null_window
        if(!resultMove){ // if not first move do null window search
            value = -alphaBetaTT(nextBoard, depth-1, -alpha-1, -alpha, !player, 0, timeIsUp);      /* search with a null window */
            if(alpha < value && value < beta){ //fail high, do research
                value = -alphaBetaTT(nextBoard, depth-1,-beta,-alpha, !player, 0, timeIsUp);
            }
        }else{
#endif
            value = -alphaBetaTT(nextBoard, depth-1,-beta,-alpha, !player, 0, timeIsUp);
#ifdef null_window
        }
#endif
        
        if(value > best){
            best = value;
#ifdef move_order_stats
            bestMoveNum = i;
#endif
            if(resultMove){
                *resultMove = moves[i];
            }
        }
        if(best > alpha){
            alpha = best;
        }
        if(best >= beta){
            if(resultMove){
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