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
#include "tt.h"
#include <iostream>

int ttCutOff;
int evalCount;
int cutOff;
int nodesTraveled;
int mtdfResearch;


//int layer;
// method call using depth 5:
// minimax = alphaBetaTT(board, 5, -MATE, +MATE);
// TT stands for Transposition Table
int alphaBetaTT(Board board, int depth, int alpha, int beta, int player, Move *resultMove, bool *timeIsUp)
{
    ++nodesTraveled;
//    for(int i = 0; i < layer; ++i){
//        std::cout << "   ";
//    }
//    std::cout << board.pointsdiff << ((ID_WE == player) ? "W" : "P") << "" << std::endl;
    
    
    int value;
#ifdef tt
    int ttType;
    int ttValue;
    Move ttMove;
    if(TT::getEntry(TT::hash(board), depth, &ttType, &ttValue, &ttMove)){
        if(BoardTools::isValidMove(board, ttMove, player)){
            if(ttType == EXACT_VALUE){
                if(resultMove){
                    *resultMove = ttMove;
                }
                ++ttCutOff;
                return ttValue;
            }
            if(ttType == LOWERBOUND && ttValue > alpha){
                alpha = ttValue;
            }else if(ttType == UPPERBOUND && ttValue < beta){
                beta = ttValue;
            }
            if(alpha >= beta){
                if(resultMove){
                    *resultMove = ttMove;
                }
                ++ttCutOff;
                return ttValue;
            }
        }
#ifdef collision_stats
        else{
            ++TT::collision_count;
        }
#endif
    }
#endif
    
    
    if(depth == 0 || board.movecount >= 60)
    {
        ++evalCount;
        value = evaluate(player, board);
        
    
        struct timespec nowTime;
        clock_gettime(Globals::clockTime, &nowTime);

        if((((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) > (TIME_FOR_CALC)){
            *timeIsUp = true;
        }
        
//        if(value <= alpha){
//            TT::storeEntry(TT::hash(*board), depth, LOWERBOUND, value, Move());
//        }else if(value >= beta){
//            TT::storeEntry(TT::hash(*board), depth, UPPERBOUND, value, Move());
//        }else{
//            TT::storeEntry(TT::hash(*board), depth, EXACT_VALUE, value, Move());
//        }
        
        return value;
    }
//    ++layer;
    int moveCount;
    Move *moves = BoardTools::generatePossibleMoves(board, player, &moveCount);
    int best = MIN_AB_VALUE-1;
    int i;
    Board nextBoard;
    for(i = 0; i < moveCount; ++i)
    {
        nextBoard = board;
        BoardTools::apply(&nextBoard, player, moves[i]);
        value = -alphaBetaTT(nextBoard, depth-1,-beta,-alpha,!player, 0, timeIsUp);
        
        if(value > best){
            best = value;
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
#ifdef tt
    if(best <= alpha){
        TT::storeEntry(TT::hash(board), depth, LOWERBOUND, best, moves[i]);
    }else if(best >= beta){
        TT::storeEntry(TT::hash(board), depth, UPPERBOUND, best, moves[i]);
    }else{
        TT::storeEntry(TT::hash(board), depth, EXACT_VALUE, best, moves[i]);
    }
#endif
    delete[] moves;
//    --layer;
    return best;
}

#ifdef mtdf
int MTDf(int f, int depth , int player, Board board, Move *resultMove, bool *timeIsUp){
    int bound[2] = {-1000, 1000}; // lower, upper
    int beta;
    --mtdfResearch;
    do {
       beta = f + (f == bound[0]);
       f = alphaBetaTT(board, depth, beta-1, beta, player, resultMove, timeIsUp);
       bound[f < beta] = f;
       ++mtdfResearch;       
    } while (bound[0] < bound[1] && !*timeIsUp);
    return f;
}
#endif

#ifdef FIXED_START_DEEP
int startDeep = START_DEEP;
#else
int startDeep = 1;
#endif

int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove)
{
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
#ifdef mtdf
        firstguess = MTDf(firstguess, d, player, board, &move, &timeIsUp);
#else
        alphaBetaTT(board, d, -1000, 1000, ID_WE, &move, &timeIsUp);
#endif
        std::cout << "Deep: " << d << " Nodes traveled: " << nodesTraveled << " Evals: " << evalCount << " CutOff: " << cutOff << " TTCutOff: " << ttCutOff << " MTDf Research: " << mtdfResearch << std::endl;
        nodesTraveled = 0;
        evalCount = 0;
        cutOff = 0;
        ttCutOff = 0;
        mtdfResearch = 0;
        
        if(timeIsUp){
            break;
        }
        //only use move of finished searches
        *resultMove = move;
    }
#ifdef DYN_START_DEEP
    startDeep = (d * 80) / 100;
#endif
    return firstguess;
}