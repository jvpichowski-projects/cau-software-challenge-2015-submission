#include "main.h"
#include <sstream>
#include <string> 


bool isInitialized = false;
bool sentMove = false;


void onPacket(char* msg){
    Fisher::Catch(msg);
}

void onTick(){
}


void onFieldRec(u_int64_t ones, u_int64_t twos, u_int64_t threes){    
    if(isInitialized) return;
        
    Globals::ones = ones;
    Globals::twos = twos;
    Globals::threes = threes;
    
    Globals::_board = Board();
    
    isInitialized = true;
    
}


/**
 * On getting moveRequestion
 */
void onMoveReq(){
    clock_gettime(Globals::clockTime, &Globals::moveReqTime); 
    
    Move move;
    //there is a bug when the game ends no move is sen
    
    Evaluation::preEvaluate(Globals::_board.movecount);

    iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move);
    
    if(!BoardTools::isValidMove(Globals::_board, move, ID_WE))
        move = BoardTools::generateGoodMove(Globals::_board, ID_WE);
    
    Ocean::Send(move);
    BoardTools::apply(&Globals::_board, ID_WE, move);
    sentMove = true;
    
    clock_gettime(Globals::clockTime, &Globals::beginningOther); 
}


/**
 * When getting last move
 * @param move
 */
void onLastMove(Move move){
    if(sentMove){
        //we sent the last move. It is already apllied to the board in onMoveReq
        sentMove = false;
        return;
    }
    
    BoardTools::apply(&Globals::_board, ID_OPPONENT, move);
    
    struct timespec nowTime;
    clock_gettime(Globals::clockTime, &nowTime);
}

int main(int argc, char** argv){   
    theBreadfish();
    
    usleep(100);
    
    Fisher::SetDigestion(&onFieldRec, &onLastMove, &onMoveReq);
    
    char* host = "127.0.0.1";
    char* port = "13050";
    std::string send("<protocol><joinPrepared reservationCode=\"");
    
    bool reservated = false;
    
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "--host") == 0)host = argv[++i];
        if(strcmp(argv[i], "--port") == 0)port = argv[++i];
        if(strcmp(argv[i], "--reservation") == 0){
            send.append(argv[++i]);
            reservated = true;
        }
        
        if(strcmp(argv[i], "-h") == 0)host = argv[++i];
        if(strcmp(argv[i], "-p") == 0)port = argv[++i];
        if(strcmp(argv[i], "-r") == 0){
            send.append(argv[++i]);
            reservated = true;
        }
    }
    
    Ocean::SwimTo(host, port, &onPacket, &onTick);
    
    if(!reservated)
        Ocean::Send("<protocol><join gameType=\"swc_2015_hey_danke_fuer_den_fisch\"/>");
    else{
        send.append("\"/>");        
        Ocean::Send(send);
    }
    
    Globals::_runningGame = true;
    Globals::_board = Board();
    
    while(Globals::_runningGame && (Globals::_board.movecount <= 60))
        Ocean::GetFood();
    
    sleep(3);
    
    Ocean::GetFood();
    
    sleep(1);
    
    Ocean::Close();
    
    return 0;
}
