#include "boardstate.h"
#include "evaluation.h"

namespace BoardTools{
    
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
        
//        if(state.movecount >= 8){
//            if(move.from == INVALID_POS && move.to == INVALID_POS){
//                return true;
//            }
//            if(move.from == INVALID_POS){
//                return false;
//            }
//            if(player == ID_WE){
//                return (state.mypos >> move.from & 1ULL) && !(state.used >> move.to & 1ULL);
//            }
//            return (state.oppos >> move.from & 1ULL) && !(state.used >> move.to & 1ULL);
//        }
//        if(move.from != INVALID_POS){
//            return false;
//        }
//        return (state.used >> move.to) & 1ULL;
    }
    
    
//    u_int64_t hash(BoardState state){
//        return state.movecount ^ state.mypos ^ state.oppos ^ state.pointsdiff ^ state.used;
//    }
    
#ifdef move_ordering
    struct MoveContainer{
        
        MoveContainer *prev;
        Move move;
        int value;
        MoveContainer *after;
        
    };
    
    void insertMove(MoveContainer *moveList, Board state, Move move, int playerId){
        //go to start of movelist
        while(moveList->prev != 0){
            moveList = moveList->prev;
        }
        MoveContainer moveConn = MoveContainer();
        moveConn.after = 0;
        moveConn.prev = 0;
        moveConn.move = move;
        apply(&state, playerId, move);
        moveConn.value = Evaluator::sortEvaluate(playerId, state);
        unapply(&state, playerId, move);
        do{
            if(moveList->value < moveConn.value){
                moveConn.after = moveList;
                moveConn.prev = moveList->prev;
                moveList->prev = &moveConn;
                moveConn.prev->after = &moveConn;
                return;
            }
            moveList = moveList->after;
        }while(moveList->after != 0);
    }
#endif
    
    Move* generateOderedSetMoves(Board state, int playerId, int *length){
         //else return setMove to firs free field
        u_int64_t freeSetPositions = Globals::ones & ~state.used;
        int freePosSize = 0;
        int* freePos = Tools::bitScan(freeSetPositions,  &freePosSize);
        //std::cout << "FreePosSize " << freePosSize << std::endl;
        *length = freePosSize;
        ValuedMove* valuedMoves = new ValuedMove[freePosSize];

        Board tmpBoard = state;
        for(int i = 0; i < freePosSize; ++i){
            #ifdef DEBUG_MOVEGEN
                    std::cout << "moveGen: Adding move: From: " << " To: " << freePos[i] << std::endl; 
            #endif
            Move m = Move();
            m.from = INVALID_POS;
            m.to = freePos[i];
            BoardTools::apply(&tmpBoard, playerId, m);
            int value = evaluate(playerId, tmpBoard);
            tmpBoard = state;            
            ValuedMove vm = ValuedMove();
            vm.move = m;
            vm.value = value;
            //valuedMoves[i] = vm;
            int b = freePosSize-1;
            for(; b > 0; --b){
                if(valuedMoves[b-1].value < value){
                    valuedMoves[b] = valuedMoves[b-1];
                }else{
                    valuedMoves[b] = vm;
                    break;
                }     
            }
            if(!b){
                valuedMoves[0] = vm;
            }
        }
        Move* moves = new Move[freePosSize];
        for(int i = 0; i < freePosSize; i++){
            moves[i] = valuedMoves[i].move;
#ifdef DEBUG_MOVE_ORDERING
            std::cout << "VM: " << valuedMoves[i].value << std::endl;
#endif
        }
        delete[] valuedMoves;
        delete[] freePos;
        return moves; 
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

            moveFields[0] = Tools::getMoveField(penguinPos[0], state.used);
            moveFields[1] = Tools::getMoveField(penguinPos[1], state.used);
            moveFields[2] = Tools::getMoveField(penguinPos[2], state.used);
            moveFields[3] = Tools::getMoveField(penguinPos[3], state.used);

            *length = Tools::popCount(moveFields[0])
                + Tools::popCount(moveFields[1])
                + Tools::popCount(moveFields[2])
                + Tools::popCount(moveFields[3]) + 1;
#ifndef move_ordering
            Move* moves = new Move[*length];
#endif
#ifdef move_ordering
            //first move is nullmove
            Move m = Move();
            m.from = INVALID_POS;
            m.to = INVALID_POS;
            
            MoveContainer moveList = MoveContainer();
            moveList.after = 0;
            moveList.prev = 0;
            moveList.move = m;
            apply(&state, playerId, m);
            moveList.value = Evaluator::sortEvaluate(playerId, state);
            unapply(&state, playerId, m);
#endif
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
#ifdef move_ordering
                    insertMove(&moveList, state, m, playerId);
#endif
#ifndef move_ordering
                    moves[++c] = m;
#endif
                }

                int twosSize = 0;
                int* twosPos = Tools::bitScan(twos, &twosSize);
                for(int k = 0; k < twosSize; k++){
                    Move m = Move();
                    m.from = penguinPos[i];
                    m.to = twosPos[k];
#ifdef move_ordering
                    insertMove(&moveList, state, m, playerId);
#endif
#ifndef move_ordering
                    moves[++c] = m;
#endif
                }

                int onesSize = 0;
                int* onesPos = Tools::bitScan(ones, &onesSize);
                for(int k = 0; k < onesSize; k++){
                    Move m = Move();
                    m.from = penguinPos[i];
                    m.to = onesPos[k];
#ifdef move_ordering
                    insertMove(&moveList, state, m, playerId);
#endif
#ifndef move_ordering
                    moves[++c] = m;
#endif
                }
            }
#ifndef move_ordering
            Move m = Move();
            m.from = INVALID_POS;
            m.to = INVALID_POS;
            moves[++c] = m;
#endif

            delete[] moveFields;

#ifdef move_ordering            
            MoveContainer *resettedList = &moveList;
            
            while(resettedList->prev != 0){
                resettedList = resettedList->prev;
            }
            Move* moves = new Move[*length];
            for(int i = 0; i < *length; ++i){
                moves[i] = resettedList->move;
                resettedList = resettedList->after;
            }
#endif
            
            return moves;
        }

        return generateOderedSetMoves(state, playerId, length);
        
//        //else return setMove to firs free field
//        u_int64_t freeSetPositions = Globals::ones & ~state.used;
//        int freePosSize = 0;
//        int* freePos = Tools::bitScan(freeSetPositions,  &freePosSize);
//        //std::cout << "FreePosSize " << freePosSize << std::endl;
//        *length = freePosSize;
//        Move* moves = new Move[freePosSize];
//
//        for(int i = 0; i < freePosSize; i++){
//            #ifdef DEBUG_MOVEGEN
//                    std::cout << "moveGen: Adding move: From: " << " To: " << freePos[i] << std::endl; 
//            #endif
//            Move m = Move();
//            m.from = INVALID_POS;
//            m.to = freePos[i];
//            moves[i] = m;
//        }
//        delete[] freePos;
//        return moves; 
    }
    
    
    Move generateGoodMove(Board state, int playerId){
        if(state.movecount >= 8){
            //std::cout << "PlayerId " << playerId << std::endl;
            u_int64_t penguinPositions = ((playerId == ID_WE) ? state.mypos : state.oppos);
            //BitBoard::printField(penguinPositions);

            int from = Tools::bitScanForward(penguinPositions);
            u_int64_t moveField = Tools::getMoveField(from, state.used);
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