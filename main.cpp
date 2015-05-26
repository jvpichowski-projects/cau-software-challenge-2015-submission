#include "main.h"
#include "tactic.h"
#include <sstream>  // Required for stringstreams
#include <string> 


bool isInitialized = false;
bool sentMove = false;


void onPacket(char* msg){
    #ifdef DEBUG_OCEAN
        std::cout << msg << std::endl;
    #endif
    Fisher::Catch(msg);
}

void onTick(){
}


void onFieldRec(u_int64_t ones, u_int64_t twos, u_int64_t threes)
{
    #ifdef DEBUG_CALLS
        cout << "DEBUG_CALLS: onFieldRec called" << endl;
    #endif
    
    if(isInitialized){
        #ifdef DEBUG_CALLS
            cout << "DEBUG_CALLS: onFieldRec canceled" << endl;
        #endif
        return;
    }
        
    Globals::ones = ones;
    Globals::twos = twos;
    Globals::threes = threes;
    
    Globals::_board = Board();
    
    isInitialized = true;
    
    //create Board
    #ifdef DEBUG_MAIN_SETFIELD
        Tools::printField(ones ^ twos ^ threes);
    #endif
}


/**
 * On getting moveRequestion
 */
void onMoveReq()
{
    
    #ifdef DEBUG_CALLS
        cout << "DEBUG_CALLS: onMoveReq called" << endl;
    #endif

    // pass the starting time to the calculate function and let the calculate function decide when to return.
    // We do not use multiple threads and therefore we can't have a coroutine checking the time.
    //Globals::moveReqTime = clock();
    clock_gettime(Globals::clockTime, &Globals::moveReqTime); 
    
    /////// TODO: MoveCalculation in here!
    
    //BitBoard::printBoard(_board);
    
    Move move;
    //there is a bug when the game ends no move is send
    //cout << "MTDf: " << Navi::alphaBetaTT(_board, 3, MIN_AB_VALUE, MAX_AB_VALUE,ID_WE, &move) << endl;
    //cout << "minimax: " << Navi::miniMax(ID_WE, 5, MIN_AB_VALUE, MAX_AB_VALUE,_board, &move) << endl;
//    std::cout << "Board pre calc: " << std::endl;
//    std::cout << "Used:" << std::endl;
//    Tools::printField(_board->used);
//    std::cout << "Mypos:" << std::endl;
//    Tools::printField(_board->mypos);
//    std::cout << "Oppos:" << std::endl;
//    Tools::printField(_board->oppos);
//    std::cout << "movecount: " << _board->movecount << " pointsdiff: " << _board->pointsdiff << std::endl;
  
    
//    u_int64_t activePlayer;
//    int rlng;
//    Move preResult;
//    
//    int preEvalRes = Tactic::preliminaries(&preResult, &rlng, &activePlayer);
//    
//    if(preEvalRes == 0)
//    {
        Evaluation::preEvaluate();
        
//        std::cout << "used: " << Globals::_board.used << std::endl;
//        Tools::printField(Globals::_board.used);
//        std::cout << "mypos: " << Globals::_board.mypos << std::endl;
//        Tools::printField(Globals::_board.mypos);
//        std::cout << "oppos: " << Globals::_board.oppos << std::endl;
//        Tools::printField(Globals::_board.oppos);
//        std::cout << "threes: " << Globals::threes << std::endl;
//        Tools::printField(Globals::threes);
//        std::cout << "twos: " << Globals::threes << std::endl;
//        Tools::printField(Globals::threes);
//        std::cout << "ones: " << Globals::threes << std::endl;
//        Tools::printField(Globals::threes);
//        
//        exit(0);
        //move = startPSearch(15, ID_WE, Globals::_board);
        //std::cout << "PSearchTo: " << (u_int64_t)startPSearch(5, ID_WE, Globals::_board).to << std::endl;
    
//        int b = 0;
//        int *penguinPosWe = Tools::fastBitScan(Globals::_board.mypos, &b);
//        b = 0;
//        int *penguinPosOp = Tools::fastBitScan(Globals::_board.oppos, &b);
//        
//        //-------------------------------generate move fields-------------------
//        
//        //wenn sich zwei linien kruezen wird das kreuz-feld nur einmal gerechnet
//        u_int64_t moveFieldsWe = Tools::genMoveField(penguinPosWe[0], Globals::_board.used) 
//                | Tools::genMoveField(penguinPosWe[1], Globals::_board.used) 
//                | Tools::genMoveField(penguinPosWe[2], Globals::_board.used)
//                | Tools::genMoveField(penguinPosWe[3], Globals::_board.used);
//        
//        u_int64_t moveFieldsOp = Tools::genMoveField(penguinPosOp[0], Globals::_board.used) 
//                | Tools::genMoveField(penguinPosOp[1], Globals::_board.used) 
//                | Tools::genMoveField(penguinPosOp[2], Globals::_board.used)
//                | Tools::genMoveField(penguinPosOp[3], Globals::_board.used);
//        
//        //count number of move fields
//        int moveFieldCount = Tools::popCount(moveFieldsWe) - Tools::popCount(moveFieldsOp);
//        int moveFieldPoints = 0;
//        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::threes)  * 3;
//        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::twos)    * 2;
//        moveFieldPoints += Tools::popCount(moveFieldsWe & Globals::ones)    * 1;
//        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::threes)  * 3;
//        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::twos)    * 2;
//        moveFieldPoints -= Tools::popCount(moveFieldsOp & Globals::ones)    * 1;
//        
//        
//        int totalReachFieldCount = 0;
//        int totalReachFieldPoints = 0;
//        int restrictedReachFieldCount = 0;
//        int restrictedReachFieldPoints = 0;
//        int ringFieldCount = 0;
//        int ringFieldPoints = 0;
//        
//        if(Globals::_board.movecount >= 8){
//            
//        //-------------------------------ring points----------------------------
//            
//            u_int64_t ringsWe = _fieldsAround[penguinPosWe[0]] | 
//                    _fieldsAround[penguinPosWe[1]] | 
//                    _fieldsAround[penguinPosWe[2]] | 
//                    _fieldsAround[penguinPosWe[3]];
//            ringsWe &= ~Globals::_board.used;
//            
//            u_int64_t ringsOp = _fieldsAround[penguinPosOp[0]] | 
//                    _fieldsAround[penguinPosOp[1]] | 
//                    _fieldsAround[penguinPosOp[2]] | 
//                    _fieldsAround[penguinPosOp[3]];
//            ringsOp &= ~Globals::_board.used;
//            
//            ringFieldCount = Tools::popCount(ringsWe) - Tools::popCount(ringsOp);
//            
//            ringFieldPoints += Tools::popCount(ringsWe & Globals::threes)  * 3;
//            ringFieldPoints += Tools::popCount(ringsWe & Globals::twos)    * 2;
//            ringFieldPoints += Tools::popCount(ringsWe & Globals::ones)    * 1;
//            ringFieldPoints -= Tools::popCount(ringsOp & Globals::threes)  * 3;
//            ringFieldPoints -= Tools::popCount(ringsOp & Globals::twos)    * 2;
//            ringFieldPoints -= Tools::popCount(ringsOp & Globals::ones)    * 1;
//            
//        //-------------------------------total reach field points---------------
//            
//            u_int64_t totalReachFieldWe = 0;
//            u_int64_t totalReachFieldOp = 0;
//            Tools::getReachableFields((~Globals::_board.used) & ~FIT,
//                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
//                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
//                    &totalReachFieldWe, &totalReachFieldOp);
//            
//            totalReachFieldCount = Tools::popCount(totalReachFieldWe) - Tools::popCount(totalReachFieldOp);
//            
//            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::threes)  * 3;
//            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::twos)    * 2;
//            totalReachFieldPoints += Tools::popCount(totalReachFieldWe & Globals::ones)    * 1;
//            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::threes)  * 3;
//            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::twos)    * 2;
//            totalReachFieldPoints -= Tools::popCount(totalReachFieldOp & Globals::ones)    * 1;
//            
//        //-------------------------------restricted reach field points----------
//            
//            u_int64_t restrictedUsedForWe = Globals::_board.used | moveFieldsOp;
//            u_int64_t restrictedUsedForOp = Globals::_board.used | moveFieldsWe;
//            u_int64_t trash = 0;
//            u_int64_t restrictedReachFieldWe = 0;
//            u_int64_t restrictedReachFieldOp = 0;
//            Tools::getReachableFields((~restrictedUsedForWe) & ~FIT,
//                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
//                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
//                    &restrictedReachFieldWe, &trash);
//            
//            trash = 0;
//            Tools::getReachableFields((~restrictedUsedForOp) & ~FIT,
//                    penguinPosWe[0], penguinPosWe[1], penguinPosWe[2], penguinPosWe[3], 
//                    penguinPosOp[0], penguinPosOp[1], penguinPosOp[2], penguinPosOp[3], 
//                    &trash, &restrictedReachFieldOp);
//            
//            restrictedReachFieldCount = Tools::popCount(restrictedReachFieldWe) - Tools::popCount(restrictedReachFieldOp);
//            
//            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::threes)  * 3;
//            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::twos)    * 2;
//            restrictedReachFieldPoints += Tools::popCount(restrictedReachFieldWe & Globals::ones)    * 1;
//            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::threes)  * 3;
//            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::twos)    * 2;
//            restrictedReachFieldPoints -= Tools::popCount(restrictedReachFieldOp & Globals::ones)    * 1;
//            
//            std::cout << "Pos We: " << std::endl;
//            Tools::printField(Globals::_board.mypos);
//            std::cout << "Restricted Used We: " << std::endl;
//            Tools::printField(restrictedUsedForWe);
//            std::cout << "Restricted Reach We: " << std::endl;
//            Tools::printField(restrictedReachFieldWe);
//            std::cout << "Pos Op: " << std::endl;
//            Tools::printField(Globals::_board.oppos);
//            std::cout << "Restricted Used Op: " << std::endl;
//            Tools::printField(restrictedUsedForOp);
//            std::cout << "Restricted Reach Op: " << std::endl;
//            Tools::printField(restrictedReachFieldOp);
//            
//            
//        }
//        
//        delete[] penguinPosWe;
//        delete[] penguinPosOp;
//        
//        int result =  Globals::_board.pointsdiff * 6                                                //6
//                    + moveFieldCount * 4 + moveFieldPoints * 1                  //4 1
//                    + ringFieldCount * 4 + ringFieldPoints * 1                  //4 1
//                    + totalReachFieldCount * 0 + totalReachFieldPoints * 0
//                    + restrictedReachFieldCount * 0 + restrictedReachFieldPoints * 0;
//        
//        std::cout << "result: " << (int64_t)result << std::endl;
//        std::cout << "points: " << (int64_t)Globals::_board.pointsdiff << std::endl;
//        std::cout << "moveFieldCount: " << (int64_t)moveFieldCount << std::endl;
//        std::cout << "moveFieldPoints: " << (int64_t)moveFieldPoints << std::endl;
//        std::cout << "ringFieldCount: " << (int64_t)ringFieldCount << std::endl;
//        std::cout << "ringFieldPoints: " << (int64_t)ringFieldPoints << std::endl;
//        std::cout << "totalReachFieldCount: " << (int64_t)totalReachFieldCount << std::endl;
//        std::cout << "totalReachFieldPoints: " << (int64_t)totalReachFieldPoints << std::endl;
//        std::cout << "restrictedReachFieldCount: " << (int64_t)restrictedReachFieldCount << std::endl;
//        std::cout << "restrictedReachFieldPoints: " << (int64_t)restrictedReachFieldPoints << std::endl;
        
        std::cout << "IT: " << iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move) << std::endl;//change first guess to 100?
        std::cout << "MT: " << (int64_t)move.from << "->" << (int64_t)move.to << std::endl;
        std::cout << "Used before: " << Globals::_board.used << std::endl;
//    }
//    else
//    {
//        move = preResult;
//        printf("\n\npppppppppppppppppppppppppppppppprevalUsed\n\n");
//    }
        
        
//    if(move.from == move.to && move.from != INVALID_POS){
//        std::cout << "======================================================" << std::endl;
//        std::cout << "Error!!! Move.from = Move.to = " << (u_int64_t)move.from << std::endl;
//        std::cout << "======================================================" << std::endl;
//        move.from = INVALID_POS;
//        move.to = INVALID_POS;
//        std::cout << "Recearching without tt..." << std::endl;
//        Globals::tt_enabled = false;
//        std::cout << "IT: " << Navi::iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move) << std::endl;
//        std::cout << "MT: " << (u_int64_t)move.from << "->" << (u_int64_t)move.to << std::endl;
//        Globals::tt_enabled = true;
//    }
    
//    std::cout << "Board post calc: " << std::endl;
//    std::cout << "Used:" << std::endl;
//    Tools::printField(_board->used);
//    std::cout << "Mypos:" << std::endl;
//    Tools::printField(_board->mypos);
//    std::cout << "Oppos:" << std::endl;
//    Tools::printField(_board->oppos);
//    std::cout << "movecount: " << _board->movecount << " pointsdiff: " << _board->pointsdiff << std::endl;
    
    if(!BoardTools::isValidMove(Globals::_board, move, ID_WE)){
        std::cout << "Found invalid move: " << ((u_int64_t)move.from) << " -> " << ((u_int64_t)move.to) << " value: " << move.value << std::endl; 
        move = BoardTools::generateGoodMove(Globals::_board, ID_WE);
        std::cout << "New move: " << ((u_int64_t)move.from) << " -> " << ((u_int64_t)move.to) << " value: " << move.value << std::endl; 
    }
    
//    struct timespec nowTime;
//    clock_gettime(Globals::clockTime, &nowTime);
//    
//    while((((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) < (TIME_FOR_CALC))
//    {
//        clock_gettime(Globals::clockTime, &nowTime);
//    }
    
    Ocean::Send(move);
    BoardTools::apply(&Globals::_board, ID_WE, move);
    sentMove = true;
    
    clock_gettime(Globals::clockTime, &Globals::beginningOther); 

    std::cout << "\n\n\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% (W) zug nr:  " << Globals::_board.movecount << "\n";
}


/**
 * When getting last move
 * @param move
 */
void onLastMove(Move move)
{
    #ifdef DEBUG_CALLS
        cout << "DEBUG_CALLS: onLastMove called: " << move.from << " -> " << move.to <<endl;
    #endif
    if(sentMove){
        //we sent the last move. It is already apllied to the board in onMoveReq
        #ifdef DEBUG_CALLS
            cout << "DEBUG_CALLS: onLastMove canceled" <<endl;
        #endif
        sentMove = false;
        return;
    }
    
    BoardTools::apply(&Globals::_board, ID_OPPONENT, move);
    
    struct timespec nowTime;
    clock_gettime(Globals::clockTime, &nowTime);
    
    std::cout << "\n\n======================Time of the others (ms): " << (((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::beginningOther.tv_sec * 1000000000 + Globals::beginningOther.tv_nsec)) / 1000000) << "\n\n\n";

    std::cout << "\n\n\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% (O) zug nr:  " << Globals::_board.movecount << "\n";
}

int main(int argc, char** argv)
{    
//    Globals::threes = 578994071121432588;
//    Globals::twos = 578994071121432588;
//    Globals::ones = 578994071121432588;
//    Globals::_board = Board();
//    
//    Move move;
//    Evaluation::preEvaluate();
//    clock_gettime(Globals::clockTime, &Globals::moveReqTime); 
//    
//    Move psearchM = startPSearch(5, ID_WE, Globals::_board);
//    std::cout << "PS: " << psearchM.value << std::endl;//change first guess to 100?
//    std::cout << "PM: " << (u_int64_t)psearchM.from << "->" << (u_int64_t)psearchM.to << std::endl;
//
//    std::cout << "IT: " << iterativeDeepening(Globals::_board, ID_WE, 5, 0, &move) << std::endl;//change first guess to 100?
//    std::cout << "MT: " << (u_int64_t)move.from << "->" << (u_int64_t)move.to << std::endl;
//        
//    exit(0);
    
    Globals::tt_enabled = true;
    
    std::cout << "Build: " << build << std::endl << std::endl;
    
    theBreadfish();
    
    usleep(100);
    
    std::cout << "Pointer Limit: " << std::numeric_limits<std::ptrdiff_t>::max() << std::endl;
    
    Fisher::SetDigestion(&onFieldRec, &onLastMove, &onMoveReq);      //set up our fisher
    
    //Ocean::SwimTo("192.168.2.103", "13050", &onPacket, &onTick);
    
    char* host = "127.0.0.1";
    char* port = "13050";
    std::string send("<protocol><joinPrepared reservationCode=\"");
    
    bool reservated = false;
    
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "--host") == 0)host = argv[++i];
        if(strcmp(argv[i], "--port") == 0)port = argv[++i];
        if(strcmp(argv[i], "--reservation") == 0)
        {
            send.append(argv[++i]);
            reservated = true;
        }
        if(strcmp(argv[i], "--points") == 0)Globals::Config::points = atoi(argv[++i]);
        if(strcmp(argv[i], "--mf") == 0)Globals::Config::moveFields = atoi(argv[++i]);
        if(strcmp(argv[i], "--rf") == 0)Globals::Config::ringFields = atoi(argv[++i]);
        if(strcmp(argv[i], "--arf") == 0)Globals::Config::aReachFields = atoi(argv[++i]);
        if(strcmp(argv[i], "--rrf") == 0)Globals::Config::rReachFields = atoi(argv[++i]);
//        if(strcmp(argv[i], "--rp1") == 0)Globals::Config::reachPoints1 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--rp2") == 0)Globals::Config::reachPoints2 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--rp3") == 0)Globals::Config::reachPoints3 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--slp1") == 0)Globals::Config::singleLinePoint1 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--slp2") == 0)Globals::Config::singleLinePoint2 = atoi(argv[++i]);
////        if(strcmp(argv[i], "--slp3") == 0)Globals::Config::singleLinePoint3 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--alp1") == 0)Globals::Config::allLinePoints1 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--alp2") == 0)Globals::Config::allLinePoints2 = atoi(argv[++i]);
//        if(strcmp(argv[i], "--alp3") == 0)Globals::Config::allLinePoints3 = atoi(argv[++i]);
        
        if(strcmp(argv[i], "-h") == 0)host = argv[++i];
        if(strcmp(argv[i], "-p") == 0)port = argv[++i];
        if(strcmp(argv[i], "-r") == 0)
        {
            send.append(argv[++i]);
            reservated = true;
        }
    }
    
    Ocean::SwimTo(host, port, &onPacket, &onTick);
    
    if(!reservated)
    {
        Ocean::Send("<protocol><join gameType=\"swc_2015_hey_danke_fuer_den_fisch\"/>");
    }
    else
    {
        send.append("\"/>");
        
        Ocean::Send(send);
    }
    
    Globals::_runningGame = true;
    Globals::_board = Board();
    
    while(Globals::_runningGame && (Globals::_board.movecount <= 60))
    {
        Ocean::GetFood();
    }
    
    
    
    
    int pointsoutside = Tools::popCount(RING1 & Globals::ones);
    pointsoutside += Tools::popCount(RING1 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING1 & Globals::threes) * 3;
    std::string stringRingGameData(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING1 & Globals::threes);
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING2 & Globals::ones);
    pointsoutside += Tools::popCount(RING2 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING2 & Globals::threes) * 3;
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING2 & Globals::threes);
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING3 & Globals::ones);
    pointsoutside += Tools::popCount(RING3 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING3 & Globals::threes) * 3;
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING3 & Globals::threes);
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING4 & Globals::ones);
    pointsoutside += Tools::popCount(RING4 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING4 & Globals::threes) * 3;
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING4 & Globals::threes);
    stringRingGameData.append(std::to_string(pointsoutside));
    stringRingGameData.append(",");
    
    if(Globals::_board.pointsdiff > 0)
        stringRingGameData.append(",W,");
    else if(Globals::_board.pointsdiff < 0)
        stringRingGameData.append(",L,");
    else if(Globals::_board.pointsdiff == 0)
        stringRingGameData.append(",N,");
    
    stringRingGameData.append(std::to_string(Globals::_board.pointsdiff));
    
    std::cout << "\n\n\nStatistikzeile_Rings: " << stringRingGameData << "\n\n\n";
    
    
    
    std::string cmdToSave("echo \"");
    cmdToSave.append(stringRingGameData);
    cmdToSave.append("\" >> /home/jonas/NetBeansProjectsSC-15-04-24/sc/dist/Release/GNU-Linux-x86/ringtestlog.csv");
    
    const char* cmdToSaveCs = cmdToSave.c_str();
    
    system(cmdToSaveCs);
    
    
    std::cout << std::endl << "Bilance: " << std::endl;
    std::cout << "CutOff: " << Globals::Log::globalCutOff << std::endl;
    std::cout << "Evals:  " << Globals::Log::globalEvalCount << std::endl;
    std::cout << "Nodes:  " << Globals::Log::globalNodesTravled << std::endl << std::endl;
    
    printf("\n\n\n========================================\nRAM-Auslastung:\n========================================\n\n\n");
    
    system("free -m");
    
    sleep(3);
    
    Ocean::GetFood();
    
    sleep(1);
    
    Ocean::Close();
    
#ifdef move_order_stats
    printMoveOrderStats();
    sleep(3);
#endif
    
    return 0;
}
