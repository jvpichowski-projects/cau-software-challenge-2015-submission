#include "main.h"
#include <sstream>
#include <string> 

//true after the board was set up
bool isInitialized = false;
//true when we have sent a move to the server and the server hasn't answered
bool sentMove = false;

/**
 * Called when a new package is recieved
 * @param msg
 */
void onPacket(char* msg){
    //handle the package
    Fisher::Catch(msg);
}

/**
 * Called on every update
 */
void onTick(){
}

/**
 * Called when the server sends a new board
 * 
 * @param ones
 * @param twos
 * @param threes
 */
void onFieldRec(u_int64_t ones, u_int64_t twos, u_int64_t threes){    
    //only change the board the first time. 
    //We are not interessted in new boards because the places of the floes stay 
    //the same during the whole game. We keep track of the used floes our self.
    if(isInitialized) return;
        
    Globals::ones = ones;
    Globals::twos = twos;
    Globals::threes = threes;
    
    Globals::_board = Board();
    
    isInitialized = true;
}


/**
 * Called when it is our turn to calculate a move and send it to the server.
 */
void onMoveReq(){
    //fetch the time
    clock_gettime(Globals::clockTime, &Globals::moveReqTime); 
    
    //create a pointer to the result move
    Move move;
    //there is a bug when the game ends no move is send
    
    //set up the right evaluation methods
    Evaluation::preEvaluate(Globals::_board.movecount);

    //start the search
    iterativeDeepening(Globals::_board, ID_WE, 60, 0, &move);
    
    //check if the move is valid otherwise select a random "ok" move
    if(!BoardTools::isValidMove(Globals::_board, move, ID_WE))
        move = BoardTools::generateGoodMove(Globals::_board, ID_WE);
    
    //send the move to the server
    Ocean::Send(move);
    //apply the move locally to keep track of the changes since the beginning of 
    //the game
    BoardTools::apply(&Globals::_board, ID_WE, move);
    //we have sent the last move
    sentMove = true;
    
    //fetch the time when the others beginn calculating
    clock_gettime(Globals::clockTime, &Globals::beginningOther); 
}


/**
 * Called when the server sends the latest move
 * 
 * @param move
 */
void onLastMove(Move move){
    if(sentMove){//TODO move apply from onMoveReq to this method because no calculation is done in the free time
        //we sent the last move. It is already apllied to the board in onMoveReq
        sentMove = false;
        return;
    }
    
    //apply the received move
    BoardTools::apply(&Globals::_board, ID_OPPONENT, move);
    
    //fetch the time to check how much time the opponent needed
    struct timespec nowTime;
    clock_gettime(Globals::clockTime, &nowTime);
}

int main(int argc, char** argv){   
    //print some very important information
    theBreadfish();
    
    usleep(100);
    
    //set up callbacks
    Fisher::SetDigestion(&onFieldRec, &onLastMove, &onMoveReq);
    
    //read connection data
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
    
    //connect to server
    Ocean::SwimTo(host, port, &onPacket, &onTick);
    
    if(!reservated)
        Ocean::Send("<protocol><join gameType=\"swc_2015_hey_danke_fuer_den_fisch\"/>");
    else{
        send.append("\"/>");        
        Ocean::Send(send);
    }
    
    //start the game
    Globals::_runningGame = true;
    //create an empty board
    Globals::_board = Board();
    
    //update state and calculate new moves while game is running
    while(Globals::_runningGame && (Globals::_board.movecount <= 60))
        //fetch the next package from the server and start calculating moves
        Ocean::GetFood();
    
    sleep(3);
    
    //fetch last package
    Ocean::GetFood();
    
    sleep(1);
    
    //close connection
    Ocean::Close();
    
    return 0;
}
