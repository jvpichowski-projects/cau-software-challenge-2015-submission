#include "main.h"
#include "tactic.h"
#include <sstream>  // Required for stringstreams
#include <string> 


bool isInitialized = false;
bool sentMove = false;


void onPacket(char* msg){
    Fisher::Catch(msg);
}

void onTick(){
}


void onFieldRec(u_int64_t ones, u_int64_t twos, u_int64_t threes)
{    
    if(isInitialized){
        return;
    }
        
    Globals::ones = ones;
    Globals::twos = twos;
    Globals::threes = threes;
    
    Globals::_board = Board();
    
    isInitialized = true;
    
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
    
    Move move;
    //there is a bug when the game ends no move is sen
    
    Evaluation::preEvaluate();

    std::cout << "IT: " << iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move) << std::endl;//change first guess to 100?
    std::cout << "MT: " << (int64_t)move.from << "->" << (int64_t)move.to << std::endl;
    std::cout << "Used before: " << Globals::_board.used << std::endl;
    
    if(!BoardTools::isValidMove(Globals::_board, move, ID_WE)){
        std::cout << "Found invalid move: " << ((u_int64_t)move.from) << " -> " << ((u_int64_t)move.to) << " value: " << move.value << std::endl; 
        move = BoardTools::generateGoodMove(Globals::_board, ID_WE);
        std::cout << "New move: " << ((u_int64_t)move.from) << " -> " << ((u_int64_t)move.to) << " value: " << move.value << std::endl; 
    }
    
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
    if(sentMove){
        //we sent the last move. It is already apllied to the board in onMoveReq
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
    
    std::cout << std::endl << "Bilance: " << std::endl;
    std::cout << "CutOff: " << Globals::Log::globalCutOff << std::endl;
    std::cout << "Evals:  " << Globals::Log::globalEvalCount << std::endl;
    std::cout << "Nodes:  " << Globals::Log::globalNodesTravled << std::endl << std::endl;
    
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
