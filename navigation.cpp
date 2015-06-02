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
        value = Evaluation::evaluate(player, board, true);
        
    
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

struct Node{
    int childsCount = 0;
    int moveCount;
    Node *childs;
    Move *moves;
    
    Board board;
    int best = MIN_AB_VALUE;
    //int alpha = -1000;
    //int beta = 1000;
};


//int nextMoveNumber[] = {0,0,0,0,0,0,0,0,0,0,0};//for 10
int *nextMoveNumber;

int pSearch(int deep, int player, Node *node, bool *changed, bool *isLast, Move *resultMove, int *bestValue, bool *timeIsUp) {
    Board board = node->board;
    if(deep <= 0  || board.movecount >= 60){
        ++evalCount;
        *changed = true;
        if(nextMoveNumber[deep] >= node->moveCount){
            nextMoveNumber[deep] = 0;
        }
        if(nextMoveNumber[deep] != 0){
            *isLast = false;
        }
        
        struct timespec nowTime;
        clock_gettime(Globals::clockTime, &nowTime);

        if((((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) > (TIME_FOR_CALC)){
            *timeIsUp = true;
        }
        
        return Evaluation::evaluate(player, board, true);//(player == ID_WE) ? board.pointsdiff : -board.pointsdiff;
    }
    
    if(nextMoveNumber[deep] >= node->moveCount){//3 = count of moves for that board
        ++nextMoveNumber[deep-1];//next deep
        nextMoveNumber[deep] = 0;
    }
    
    if(node->childsCount <= nextMoveNumber[deep]){
    //maybe use special board for leaf node
        Board nextBoard = board;
        BoardTools::apply(&nextBoard, player, node->moves[nextMoveNumber[deep]]);
        //Board nextBoard = applyMove(board, player, node->moves[nextMoveNumber[deep]]);
        Node childNode = Node();
        childNode.board = nextBoard;
        //childNode.moves = generateMoves(!player);
        childNode.moves = BoardTools::generatePossibleMoves(nextBoard, !player, &childNode.moveCount);
        childNode.childs = new Node[childNode.moveCount];
//        childNode.alpha = -node->beta;
//        childNode.beta = -node->alpha;
        node->childs[nextMoveNumber[deep]] = childNode;
        ++node->childsCount;
    }
//    std::cout << "nextMn" << nextMoveNumber[deep] << std::endl;
    int value = -pSearch(deep-1, !player, &node->childs[nextMoveNumber[deep]], changed, isLast, 0, 0, timeIsUp);
    if(&changed){
        if(value > node->best){
            node->best = value;
            if(resultMove){
                *resultMove = node->moves[nextMoveNumber[deep]]; 
                *bestValue = value;
            }
            *changed = true;
        }
//don't know if it is a good idea. also it needs a correct finish code
//        if(node->best > node->alpha){
//            node->alpha = node->best;
//        }
//        if(node->best >= node->beta){
//            ++cutOff;
//            
//            ++nextMoveNumber[deep-1];//next deep
//            nextMoveNumber[deep] = 0;
//        }
    }
    
    if(nextMoveNumber[deep] != 0){
        //wasn't resetted -> wasn't last
        *isLast = false;
    }
    
    return value;
}

Move startPSearch(int maxDeep, int player, Board board){
    nextMoveNumber = new int[maxDeep+1];//why??????
    for(int i = 0; i < maxDeep+1; ++i){
        nextMoveNumber[i] = 0;
    }
    Move rMove;
    Node rootNode = Node();
    rootNode.board = board;
    rootNode.moves = BoardTools::generatePossibleMoves(board, !player, &rootNode.moveCount);
    rootNode.childs = new Node[rootNode.moveCount];
    bool isLast = false;//maby dangerous if only one move is available
    int bestValue;
    bool timeIsUp = false;
    
    while(true){
        bool changed = false;
        int value = pSearch(maxDeep, player, &rootNode, &changed, &isLast, &rMove, &bestValue, &timeIsUp);
        if(nextMoveNumber[maxDeep] >= rootNode.moveCount){//3 = count of moves for that board
            ++nextMoveNumber[maxDeep-1];//next deep
            nextMoveNumber[maxDeep] = 0;
        }
        if(nextMoveNumber[maxDeep] != 0){
            //wasn't resetted -> wasn't last
            isLast = false;
        }
        if(isLast || timeIsUp){
            break;
        }
        ++nextMoveNumber[maxDeep];
        isLast = true;
    }
    std::cout << "PSearch value: " << bestValue << std::endl;
    
    return rMove;
}