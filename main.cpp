#include "main.h"
#include "tactic.h"
#include <sstream>  // Required for stringstreams
#include <string> 


bool isInitialized = false;
bool sentMove = false;

std::string intToString ( int number )
{
  std::ostringstream oss;

  // Works just like cout
  oss<< number;

  // Return the underlying string
  return oss.str();
}

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
    
        std::cout << "IT: " << iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move) << std::endl;//change first guess to 100?
        std::cout << "MT: " << (u_int64_t)move.from << "->" << (u_int64_t)move.to << std::endl;
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
int64_t a_fieldsAround[60] = {
            /* |  00 */0x182ULL,             /* |  01 */0x305ULL,             /* |  02 */0x60AULL,             /* |  03 */0xC14ULL,
            /* |  04 */0x1828ULL,            /* |  05 */0x3050ULL,            /* |  06 */0x6020ULL,            /* |  07 */0x8101ULL,
            /* |  08 */0x18283ULL,           /* |  09 */0x30506ULL,           /* |  10 */0x60A0CULL,           /* |  11 */0xC1418ULL,
            /* |  12 */0x182830ULL,          /* |  13 */0x305060ULL,          /* |  14 */0x202040ULL,          /* |  15 */0xC10180ULL,
            /* |  16 */0x1828300ULL,         /* |  17 */0x3050600ULL,         /* |  18 */0x60A0C00ULL,         /* |  19 */0xC141800ULL,
            /* |  20 */0x18283000ULL,        /* |  21 */0x30106000ULL,        /* |  22 */0x40808000ULL,        /* |  23 */0xC1418000ULL,
            /* |  24 */0x182830000ULL,       /* |  25 */0x305060000ULL,       /* |  26 */0x60A0C0000ULL,       /* |  27 */0xC14180000ULL,
            /* |  28 */0x1828300000ULL,      /* |  29 */0x1010200000ULL,      /* |  30 */0x6080C00000ULL,      /* |  31 */0xC141800000ULL,
            /* |  32 */0x18283000000ULL,     /* |  33 */0x30506000000ULL,     /* |  34 */0x60A0C000000ULL,     /* |  35 */0xC1418000000ULL,
            /* |  36 */0x180830000000ULL,    /* |  37 */0x204040000000ULL,    /* |  38 */0x60A0C0000000ULL,    /* |  39 */0xC14180000000ULL,
            /* |  40 */0x1828300000000ULL,   /* |  41 */0x3050600000000ULL,   /* |  42 */0x60A0C00000000ULL,   /* |  43 */0xC141800000000ULL,
            /* |  44 */0x8081000000000ULL,   /* |  45 */0x30406000000000ULL,  /* |  46 */0x60A0C000000000ULL,  /* |  47 */0xC1418000000000ULL,
            /* |  48 */0x182830000000000ULL, /* |  49 */0x305060000000000ULL, /* |  50 */0x60A0C0000000000ULL, /* |  51 */0xC04180000000000ULL,
            /* |  52 */0x20200000000000ULL,  /* |  53 */0x50600000000000ULL,  /* |  54 */0xA0C00000000000ULL,  /* |  55 */0x141800000000000ULL,
            /* |  56 */0x283000000000000ULL, /* |  57 */0x506000000000000ULL, /* |  58 */0xA0C000000000000ULL, /* |  59 */0x408000000000000ULL   };

struct foundNode{
    u_int64_t field;
    foundNode *nextNode;    
};
int main(int argc, char** argv)
{          
    u_int64_t testField = 244892066650788096ULL;
    foundNode *founds = new foundNode();
    founds->field = 0;
    founds->nextNode = 0;
    for(int i = 0; i < 60; ++i){
        if(testField & (1ULL << i)){
            std::cout << "Found set position" << std::endl;
            u_int64_t around = a_fieldsAround[i];
            foundNode *nextFound = founds;
            u_int64_t *firstfoundfield = 0;
            while(nextFound != 0){
                foundNode *prev = nextFound;
                nextFound = nextFound->nextNode;
                if(nextFound == 0){
                    std::cout << "Reached end of foundlist" << std::endl;
                    nextFound = prev;
                    break;
                }
                if(nextFound->field & around){
                    std::cout << "Found fitting field" << std::endl;
                    nextFound->field |= (1ULL << i);
                    if(firstfoundfield != 0){
                        std::cout << "Connecting fields" << std::endl;
                        *firstfoundfield |= nextFound->field;
                        prev->nextNode = nextFound->nextNode;
                        foundNode *toDelete = nextFound;
                        nextFound = nextFound->nextNode;
                        delete toDelete;
                    }else{
                        firstfoundfield = &nextFound->field;
                    }
                }
            }            
            if(firstfoundfield == 0){
                std::cout << "created new node" << std::endl;
                foundNode *newFound = new foundNode();
                newFound->field = (1ULL << i);
                newFound->nextNode = 0;
                nextFound->nextNode = newFound;
            }
        }
    }
    while(founds != 0){
        Tools::printField(founds->field);
        foundNode *prev = founds;
        founds = founds->nextNode;
        delete prev;
    }
    
    
    Tools::printField(testField);
//    Tools::printField(Tools::genMoveField(33, 468356338187396469ULL));
//    Tools::printField(Tools::genMoveField(33, 0));
    
    return 0;
    
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
    std::string stringRingGameData(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING1 & Globals::threes);
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING2 & Globals::ones);
    pointsoutside += Tools::popCount(RING2 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING2 & Globals::threes) * 3;
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING2 & Globals::threes);
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING3 & Globals::ones);
    pointsoutside += Tools::popCount(RING3 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING3 & Globals::threes) * 3;
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING3 & Globals::threes);
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING4 & Globals::ones);
    pointsoutside += Tools::popCount(RING4 & Globals::twos) * 2;
    pointsoutside += Tools::popCount(RING4 & Globals::threes) * 3;
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    pointsoutside = Tools::popCount(RING4 & Globals::threes);
    stringRingGameData.append(intToString(pointsoutside));
    stringRingGameData.append(",");
    
    if(Globals::_board.pointsdiff > 0)
        stringRingGameData.append(",W,");
    else if(Globals::_board.pointsdiff < 0)
        stringRingGameData.append(",L,");
    else if(Globals::_board.pointsdiff == 0)
        stringRingGameData.append(",N,");
    
    stringRingGameData.append(intToString(Globals::_board.pointsdiff));
    
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
