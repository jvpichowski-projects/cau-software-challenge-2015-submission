#include "main.h"

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
    
    
    std::cout << "IT: " << iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move) << std::endl;
    std::cout << "MT: " << (u_int64_t)move.from << "->" << (u_int64_t)move.to << std::endl;
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
        move = BoardTools::generateGoodMove(Globals::_board, ID_WE);
    }
    
    struct timespec nowTime;
    clock_gettime(Globals::clockTime, &nowTime);
    
    while((((nowTime.tv_sec * 1000000000 + nowTime.tv_nsec) - (Globals::moveReqTime.tv_sec * 1000000000 + Globals::moveReqTime.tv_nsec)) / 1000000) < (TIME_FOR_CALC))
    {
        clock_gettime(Globals::clockTime, &nowTime);
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
    
    printf("\n\n\n========================================\nRAM-Auslastung:\n========================================\n\n\n");
    
    system("free -m");
    
    sleep(3);
    
    Ocean::GetFood();
    
    sleep(1);
    
    Ocean::Close();
    
#ifdef tt
    
    std::cout << std::endl;
    std::cout << "TT Stats" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "lookup_count " << TT::lookup_count << std::endl;
    std::cout << "positve_lookup_count " << TT::positiv_lookup_count << std::endl;
    std::cout << "min collision_count " << TT::collision_count << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    
#endif /* tt */
    
#ifdef move_order_stats
    printMoveOrderStats();
    sleep(3);
#endif
    
    return 0;
}
