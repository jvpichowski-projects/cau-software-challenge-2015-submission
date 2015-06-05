#include "boardstate.h"
#include "evaluation.h"

namespace BoardTools{
    
    /**
     * 
     * @param state needs to have initialized move fields!
     * @param playerId
     * @param length
     * @return 
     */
//    Board* generatePossibleBoards(Board state, int playerId, int *length){
//        if(state.movecount >= 8){
//            *length = state.allMoveFieldCount[playerId];
//            
//            if(*length <= 0){
//                Board *boards = new Board[1];
//                *length = 1;
//                Move m = Move();
//                m.from = INVALID_POS;
//                m.to = INVALID_POS;
//                Board newBoard = state;
//                apply(&newBoard, playerId, m);
//                genMoveField(&newBoard);
//                newBoard.lastMove = m;
//                boards[0] = newBoard;
//                return boards;
//            }
//            
//            Board *boards = new Board[1];
//            
//            int c = 0;
//            for(int f = 0; f < 4; ++f){
//                u_int64_t moveField = state.moveField[playerId][f];
//                int moveCount = 0;
//                int* moves = Tools::bitScan(moveField, &moveCount);
//                for(int m = 0; m < moveCount; ++m){
//                    Move mov = Move();
//                    mov.from = state.pos[playerId][f];
//                    mov.to = moves[m];
//                    Board newBoard = state;
//                    apply(&newBoard, playerId, mov);
//                    genMoveField(&newBoard);
//                    newBoard.lastMove = mov;
//                    boards[c] = newBoard;
//                    ++c;
//                }
//                delete[] moves;
//            }
//            
//            return boards;
//        }
//        
//        
//        //else return setMove
//        u_int64_t freeSetPositions = Globals::ones & ~state.used;
//        int freePosSize = 0;
//        int* freePos = Tools::bitScan(freeSetPositions,  &freePosSize);
//        //std::cout << "FreePosSize " << freePosSize << std::endl;
//        *length = 1;
//        
//        Board *boards = new Board[1];
//        Move m = Move();
//        m.from = INVALID_POS;
//        m.to = freePos[0];
//        Board newBoard = state;
//        apply(&newBoard, playerId, m);
//        newBoard.lastMove = m;
//        boards[0] = newBoard;
//        delete[] freePos;
//        return boards; 
//    }
    
//    //only works in move phase
//    void genMoveField(Board *board){
//        
//        int b = 0;
//        int *penguinPosWe = Tools::fastBitScan(board->mypos, &b);
//        if(b < 4){
//            return;
//        }
//        b = 0;
//        int *penguinPosOp = Tools::fastBitScan(board->oppos, &b);
//        if(b < 4){
//            return;
//        }
//        
//        board->pos[ID_WE][0] = penguinPosWe[0];
//        board->pos[ID_WE][1] = penguinPosWe[1];
//        board->pos[ID_WE][2] = penguinPosWe[2];
//        board->pos[ID_WE][3] = penguinPosWe[3];
//        board->pos[ID_OPPONENT][0] = penguinPosOp[0];
//        board->pos[ID_OPPONENT][1] = penguinPosOp[1];
//        board->pos[ID_OPPONENT][2] = penguinPosOp[2];
//        board->pos[ID_OPPONENT][3] = penguinPosOp[3];
//        //-------------------------------generate move fields-------------------
//        
//        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
//        board->moveField[ID_WE][0] = Tools::genMoveField(board->pos[ID_WE][0], board->used); 
//        board->moveField[ID_WE][1] = Tools::genMoveField(board->pos[ID_WE][1], board->used); 
//        board->moveField[ID_WE][2] = Tools::genMoveField(board->pos[ID_WE][2], board->used); 
//        board->moveField[ID_WE][3] = Tools::genMoveField(board->pos[ID_WE][3], board->used); 
//        
//        board->moveFieldAll[ID_WE] = 
//                board->moveField[ID_WE][0] |
//                board->moveField[ID_WE][1] |
//                board->moveField[ID_WE][2] |
//                board->moveField[ID_WE][3];
//        
//        board->allMoveFieldCount[ID_WE] = Tools::popCount(board->moveFieldAll[ID_WE]);
//        
//        board->moveField[ID_OPPONENT][0] = Tools::genMoveField(board->pos[ID_OPPONENT][0], board->used);
//        board->moveField[ID_OPPONENT][1] = Tools::genMoveField(board->pos[ID_OPPONENT][1], board->used);
//        board->moveField[ID_OPPONENT][2] = Tools::genMoveField(board->pos[ID_OPPONENT][2], board->used);
//        board->moveField[ID_OPPONENT][3] = Tools::genMoveField(board->pos[ID_OPPONENT][3], board->used);
//        
//        board->moveFieldAll[ID_OPPONENT] =
//                board->moveField[ID_OPPONENT][0] |
//                board->moveField[ID_OPPONENT][1] |
//                board->moveField[ID_OPPONENT][2] |
//                board->moveField[ID_OPPONENT][3];
//        
//        board->allMoveFieldCount[ID_OPPONENT] = Tools::popCount(board->moveFieldAll[ID_OPPONENT]);
//        
//        delete[] penguinPosWe;
//        delete[] penguinPosOp;
//    }
    
    int apply(Board* state, int playerId, Move move){
        ++state->movecount;
//        if(move.to >= 60){
//            return state->movecount;
//        }
        state->used |= 1ULL << move.to;
//        state->used &= ~FIT;
        if(playerId == ID_WE){
            state->mypos |= 1ULL << move.to;
            state->mypos &= ~(1ULL << move.from);
            state->pointsdiff += ((Globals::threes >> move.to) & 1ULL) * 3 + ((Globals::twos >> move.to) & 1ULL) * 2 + ((Globals::ones >> move.to) & 1ULL);
        }else{
            state->oppos |= 1ULL << move.to;
            state->oppos &= ~(1ULL << move.from);
            state->pointsdiff -= ((Globals::threes >> move.to) & 1ULL) * 3 + ((Globals::twos >> move.to) & 1ULL) * 2 + ((Globals::ones >> move.to) & 1ULL);
        }
        return state->movecount;
    }
    
    int unapply(Board* state, int playerId, Move move){
        --state->movecount;
//        if(move.to >= 60){
//            return state->movecount;
//        }
        state->used &= ~(1ULL << move.to);
//        state->used &= ~FIT;
        if(playerId == ID_WE){
            state->mypos &= ~(1ULL << move.to);
            state->mypos |= 1ULL << move.from;
            state->pointsdiff -= ((Globals::threes >> move.to) & 1ULL) * 3 + ((Globals::twos >> move.to) & 1ULL) * 2 + ((Globals::ones >> move.to) & 1ULL);
        }else{
            state->oppos &= ~(1ULL << move.to);
            state->oppos |= 1ULL << move.from;
            state->pointsdiff += ((Globals::threes >> move.to) & 1ULL) * 3 + ((Globals::twos >> move.to) & 1ULL) * 2 + ((Globals::ones >> move.to) & 1ULL);
        }
        return state->movecount;
    }
    
    
    bool isValidMove(Board state, Move move, int player){
        return 
                //move must be valid
                (move.from <= INVALID_POS && move.to <= INVALID_POS) &&
                //if mc < 8                is setmove                                                 field is free
                (((state.movecount < 8) && (move.from == INVALID_POS) && (move.to != INVALID_POS) && !(state.used >> move.to & 1ULL)) 
                //else if mc >= 8               is no setmove
                || ((state.movecount >= 8) && !(move.from == INVALID_POS && move.to != INVALID_POS) && 
                //is nullmove
                (((move.from == INVALID_POS) && (move.to == INVALID_POS))
                //or is runmove to free field             if our turn          and own field
                || (!(state.used >> move.to & 1ULL) && (((player == ID_WE) && (state.mypos >> move.from & 1ULL))
                //           or if opponents turn and its field
                        || ((player != ID_WE) && (state.oppos >> move.from & 1ULL)))))));
    }
    
//    int insertSetBoard(Board* valuedBoards, Board board, int playerId, int length){
//        genMoveField(&board);
//        int value = Evaluation::fastEval(playerId, board);
//        board.fastValue = value;
//        int b = length-1;
//         
//        //only sort boards in usefull range
//        //untested
////#ifdef limit_move_gen
////        if(b > MOVE_GEN_LIMIT){
////            b = MOVE_GEN_LIMIT;
////        }
////#endif
//        for(; b > 0; --b){
//            if(valuedBoards[b-1].fastValue < value){
//                valuedBoards[b] = valuedBoards[b-1];
//            }else{
//                valuedBoards[b] = board;
//                break;
//            }     
//        }
//        if(!b){
//            valuedBoards[0] = board;
//        }
//        return value;
//    }
//    
//    int insertBoard(Board* valuedBoards, Board board, int playerId, int length){
//        int value = Evaluation::fastEval(playerId, board);
//        board.fastValue = value;
//        int b = length-1;
//         
//        //only sort boards in usefull range
//        //untested
////#ifdef limit_move_gen
////        if(b > MOVE_GEN_LIMIT){
////            b = MOVE_GEN_LIMIT;
////        }
////#endif
//        for(; b > 0; --b){
//            if(valuedBoards[b-1].fastValue < value){
//                valuedBoards[b] = valuedBoards[b-1];
//            }else{
//                valuedBoards[b] = board;
//                break;
//            }     
//        }
//        if(!b){
//            valuedBoards[0] = board;
//        }
//        return value;
//    }
//    
//    /**
//     * 
//     * @param state needs to have initialized move fields!
//     * @param playerId
//     * @param length
//     * @return 
//     */
//    Board* generatePossibleBoards(Board state, int playerId, int *length){
//        if(state.movecount >= 8){
//            *length = state.allMoveFieldCount[playerId];
//        
//            if(*length <= 0){
//                *length = 1;
//                Board *boards = new Board[*length];
//                Move m = Move();
//                m.from = INVALID_POS;
//                m.to = INVALID_POS;
//                Board newBoard = state;
//                apply(&newBoard, playerId, m);
//                genMoveField(&newBoard);
//                newBoard.lastMove = m;
//                boards[0] = newBoard;
//                return boards;
//            }
//            
//#ifdef limit_move_gen
//            if(*length > MOVE_GEN_LIMIT){
//                *length = MOVE_GEN_LIMIT;
//            }
//#endif
//            Board *boards = new Board[*length];
//
//            for(int i = 0; i < 4; ++i){
//                Move m = Move();
//                m.from = state.pos[playerId][i];
//                
//                u_int64_t threes = state.pos[playerId][i] & Globals::threes;
//                
//                int threesSize = 0;
//                int* threesPos = Tools::bitScan(threes, &threesSize);
//                for(int k = 0; k < threesSize; k++){
//                    m.to = threesPos[k];
//                    Board newBoard = state;
//                    apply(&newBoard, playerId, m);
//                    genMoveField(&newBoard);
//                    newBoard.lastMove = m;
//                    insertBoard(boards, newBoard, playerId, *length);
//                }
//                delete[] threesPos;
//            }
//            
//            for(int i = 0; i < 4; ++i){
//                Move m = Move();
//                m.from = state.pos[playerId][i];
//                
//                u_int64_t twos = state.pos[playerId][i] & Globals::twos;
//                
//                int twosSize = 0;
//                int* twosPos = Tools::bitScan(twos, &twosSize);
//                for(int k = 0; k < twosSize; k++){
//                    m.to = twosPos[k];
//                    Board newBoard = state;
//                    apply(&newBoard, playerId, m);
//                    genMoveField(&newBoard);
//                    newBoard.lastMove = m;
//                    insertBoard(boards, newBoard, playerId, *length);
//                }
//                delete[] twosPos;
//            }
//            
//            for(int i = 0; i < 4; ++i){
//                Move m = Move();
//                m.from = state.pos[playerId][i];
//                
//                u_int64_t ones = state.pos[playerId][i] & Globals::ones;
//                
//                int onesSize = 0;
//                int* onesPos = Tools::bitScan(ones, &onesSize);
//                for(int k = 0; k < onesSize; k++){
//                    m.to = onesPos[k];
//                    Board newBoard = state;
//                    apply(&newBoard, playerId, m);
//                    genMoveField(&newBoard);
//                    newBoard.lastMove = m;
//                    insertBoard(boards, newBoard, playerId, *length);
//                }
//                delete[] onesPos;
//            }
//            
//            return boards;
//        }
//        
//        //else return setMove
//        u_int64_t freeSetPositions = Globals::ones & ~state.used;
//        int freePosSize = 0;
//        int* freePos = Tools::bitScan(freeSetPositions,  &freePosSize);
//        //std::cout << "FreePosSize " << freePosSize << std::endl;
//        *length = freePosSize;
//#ifdef limit_move_gen
//        if(*length > MOVE_GEN_LIMIT){
//            *length = MOVE_GEN_LIMIT;
//        }
//#endif
//        Board *boards = new Board[*length];
//        Move m = Move();
//        m.from = INVALID_POS;
//        for(int i = 0; i < freePosSize; i++){
//            m.to = freePos[i];
//            Board newBoard = state;
//            apply(&newBoard, playerId, m);
//            newBoard.lastMove = m;
//            insertBoard(boards, newBoard, playerId, *length);
//        }
//        delete[] freePos;
//        return boards; 
//    }
    
    /**
     * 
     * @param valuedMoves the pointer to the first element of the move array in which the move should be insert
     * @param m the move
     * @param state
     * @param playerId 
     * @param length of move array
     * @return value of move
     */
    int insertMove(Move* valuedMoves, Move m, Board state, int playerId, int length){
        Board tmpBoard = state;
        BoardTools::apply(&tmpBoard, playerId, m);
        int value = Evaluation::fastEval(playerId, tmpBoard);  
        m.value = value;
        //valuedMoves[i] = vm;
        int b = length-1;
        
        //untested
#ifdef limit_move_gen
        if(b > MOVE_GEN_LIMIT){
            b = MOVE_GEN_LIMIT;
        }
#endif
        for(; b > 0; --b){
            if(valuedMoves[b-1].value < value){
                valuedMoves[b] = valuedMoves[b-1];
            }else{
                valuedMoves[b] = m;
                break;
            }     
        }
        if(!b){
            valuedMoves[0] = m;
        }
        return value;
    }
    
    /**
    //This method returns all valid moves for the player. 
    //The moves should be sorted best to bad. Consequently the NullMove is the lastMove
     * 
     * @param playerId
     * @param length the length of the resulting array
     * @return an array of all valid moves for the player preferable sorted form best to bad
     */
    Move* generatePossibleMoves(Board state, int playerId, int *length){
        //if not in set mode return null move
        //std::cout << "Gmc " << getMoveCount() << std::endl;
        if(state.movecount >= 8){
            //std::cout << "PlayerId " << playerId << std::endl;
            u_int64_t penguinPositions = ((playerId == ID_WE) ? state.mypos : state.oppos);
            //BitBoard::printField(penguinPositions);
            int l = 0;
            int* penguinPos = Tools::fastBitScan(penguinPositions, &l);

            u_int64_t *moveFields = new u_int64_t[4];

            moveFields[0] = Tools::genMoveField(penguinPos[0], state.used);
            moveFields[1] = Tools::genMoveField(penguinPos[1], state.used);
            moveFields[2] = Tools::genMoveField(penguinPos[2], state.used);
            moveFields[3] = Tools::genMoveField(penguinPos[3], state.used);

            *length = Tools::popCount(moveFields[0])
                + Tools::popCount(moveFields[1])
                + Tools::popCount(moveFields[2])
                + Tools::popCount(moveFields[3]);
            bool nullMove = false;
            if(*length <= 0){
                *length = 1;
                nullMove = true;
            }
            Move* moves = new Move[*length];
            //std::cout << "Length: " << *length << std::endl;

            int c = -1;
            for(int i = 0; i < 4; i++){
                u_int64_t threes = Globals::threes & moveFields[i];
                u_int64_t twos = Globals::twos & moveFields[i];
                u_int64_t ones = Globals::ones & moveFields[i];

                //std::cout << "penguin" << i << " - " << penguinPos[i] << std::endl;

                int threesSize = 0;
                int* threesPos = Tools::bitScan(threes, &threesSize);
                for(int k = 0; k < threesSize; k++){
                    Move m = Move();
                    m.from = penguinPos[i];
                    m.to = threesPos[k];
#ifdef ordered_runmoves
                    insertMove(moves, m, state, playerId, *length);
#else
                    moves[++c] = m;
#endif
                }
                delete[] threesPos;

                int twosSize = 0;
                int* twosPos = Tools::bitScan(twos, &twosSize);
                for(int k = 0; k < twosSize; k++){
                    Move m = Move();
                    m.from = penguinPos[i];
                    m.to = twosPos[k];
#ifdef ordered_runmoves
                    insertMove(moves, m, state, playerId, *length);
#else
                    moves[++c] = m;
#endif
                }
                delete[] twosPos;

                int onesSize = 0;
                int* onesPos = Tools::bitScan(ones, &onesSize);
                for(int k = 0; k < onesSize; k++){
                    Move m = Move();
                    m.from = penguinPos[i];
                    m.to = onesPos[k];
#ifdef ordered_runmoves
                    insertMove(moves, m, state, playerId, *length);
#else
                    moves[++c] = m;
#endif
                }
                delete[] onesPos;
                
            }
            if(nullMove){
                Move m = Move();
                m.from = INVALID_POS;
                m.to = INVALID_POS;
#ifdef ordered_runmoves
                insertMove(moves, m, state, playerId, *length);
#else
                    moves[++c] = m;
#endif
            }
            
            delete[] moveFields, penguinPos;
            
#ifdef DEBUG_MOVE_ORDERING
        for(int i = 0; i < *length; i++){
            std::cout << "VM: " << moves[i].value << std::endl;
        }
#endif
#ifdef limit_move_gen
            if(*length > MOVE_GEN_LIMIT){
                *length = MOVE_GEN_LIMIT;
            }
#endif
            return moves;
        }
        //else return setMove
        u_int64_t freeSetPositions = Globals::ones & ~state.used;
        int freePosSize = 0;
        int* freePos = Tools::bitScan(freeSetPositions,  &freePosSize);
        //std::cout << "FreePosSize " << freePosSize << std::endl;
        *length = freePosSize;
        Move* moves = new Move[freePosSize];

        for(int i = 0; i < freePosSize; i++){
            #ifdef DEBUG_MOVEGEN
                    std::cout << "moveGen: Adding move: From: " << " To: " << freePos[i] << std::endl; 
            #endif
            Move m = Move();
            m.from = INVALID_POS;
            m.to = freePos[i];
#ifdef ordered_setmoves
            insertMove(moves, m, state, playerId, freePosSize);
#else
            moves[i] = m;
#endif
        }
        
#ifdef DEBUG_MOVE_ORDERING
        for(int i = 0; i < freePosSize; i++){
            std::cout << "VM: " << moves[i].value << std::endl;
        }
#endif
#ifdef limit_move_gen
            if(*length > MOVE_GEN_LIMIT){
                *length = MOVE_GEN_LIMIT;
            }
#endif
        delete[] freePos;
        return moves; 
    }
    
    
    Move generateGoodMove(Board state, int playerId){
        if(state.movecount >= 8){
            //std::cout << "PlayerId " << playerId << std::endl;
            u_int64_t penguinPositions = ((playerId == ID_WE) ? state.mypos : state.oppos);
            //BitBoard::printField(penguinPositions);

            int from = Tools::bitScanForward(penguinPositions);
            u_int64_t moveField = Tools::genMoveField(from, state.used);
            int to;
            if(Globals::threes & moveField){
                to = Tools::bitScanForward(Globals::threes & moveField);
            }else if(Globals::twos & moveField){
                to = Tools::bitScanForward(Globals::twos & moveField);
            }else if(Globals::ones & moveField){
                to = Tools::bitScanForward(Globals::ones & moveField);
            }else{
                //null move mustn't be the only move but we don'T want to look 
                //at evry penguin. and when one penguin is catched it is probably 
                //that the game ends son and this search isn't important anymore.
                to = INVALID_POS;
                from = INVALID_POS;
            }
            Move result = Move();
            result.from = from;
            result.to = to;
            return result;
        }
        Move result = Move();
        result.from = INVALID_POS;
        result.to = Tools::bitScanForward(Globals::ones & ~state.used);
        return result;
    }
};