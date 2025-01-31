#include "network.h"

/*****************************************************************Ocean********/

#define RCVBUFSIZE 8192

namespace Ocean{
    int _socket;
    std::string packetBuffer = "";
    Ocean::OnTick _ont;
    Ocean::OnReceive _onr;
}

void Ocean::SwimTo(char* ip, char* port, Ocean::OnReceive onr, Ocean::OnTick ont)
{    
    std::cout << "Connecting to " << ip << ":" << port << "..." << std::endl;
    
    _ont = ont;
    _onr = onr;
    struct addrinfo host_info;      
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof host_info);
    
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    
    int status = getaddrinfo(ip, port, &host_info, &host_info_list);
    if(status != 0){
        std::cout << "getaddrinfo error" << gai_strerror(status);
    }
  
    _socket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
    if (_socket == -1)  std::cout << "socket error " ;
    
    status = connect(_socket, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "connect error" ;
    
    fcntl(_socket, F_SETFL, O_NONBLOCK);
}

void Ocean::GetFood()
{
    ssize_t bytes_recieved;
    char incoming_data_buffer[RCVBUFSIZE];

    bytes_recieved = recv(_socket, incoming_data_buffer, RCVBUFSIZE, 0);
    
    
    if(bytes_recieved > 0){
        incoming_data_buffer[bytes_recieved] = '\0';
        
        packetBuffer += incoming_data_buffer;
                
        if((packetBuffer.length() > 5 && packetBuffer.substr(packetBuffer.length() - 6, 6) == "/room>")
                || (packetBuffer.length() > 9 && packetBuffer.substr(packetBuffer.length() - 10, 10) == "/protocol>")
                || (packetBuffer.length() > 6 && packetBuffer.substr(packetBuffer.length() - 7, 7) == "/room>\n")
                || (packetBuffer.length() > 10 && packetBuffer.substr(packetBuffer.length() - 11, 11) == "/protocol>\n")){
            char* packeBufferAr = (char*)packetBuffer.c_str();
            _onr(packeBufferAr);
            packetBuffer = "";
        }
    }

    _ont();
}

void Ocean::Send(char* msg){
    send(_socket, msg, strlen(msg), 0);
}

void Ocean::Send(std::string msg){
    char* cstr = (char*)msg.c_str();
    send(_socket, cstr, strlen(cstr), 0);
}

void Ocean::Send(Move move){
    if(Tools::isInvalid(move.to)){
        //send NullMove
        
        std::string send("<room roomId=\"");
        send.append(Fisher::cpRid);
        send.append("\">\n\t<data class=\"NullMove\"/>\n</room>\n");
        
        Send(send);
    }else if(Tools::isInvalid(move.from)){
        char* zahl = new char[2];
        zahl[1] = '\0';
        
        int* pos = Tools::getPos(move.to);
        int x = pos[0];
        int y = pos[1];
        delete[] pos;
        //send SetMove
        
        std::string send("<room roomId=\"");
        send.append(Fisher::cpRid);
        send.append("\">\n\t<data class=\"SetMove\" setX=\"");
        zahl[0] = (char)('0' + x);
        send.append(zahl);
        send.append("\" setY=\"");
        zahl[0] = (char)('0' + y);
        send.append(zahl);
        send.append("\"/>\n</room>");
        
        delete[] zahl;
        
        Ocean::Send(send);
    }else{
        char* zahl = new char[2];
        zahl[1] = '\0';
        
        int* toPos = Tools::getPos(move.to);
        int x_to = toPos[0];
        int y_to = toPos[1];
        delete[] toPos;
        int* fromPos = Tools::getPos(move.from);
        int x_from = fromPos[0];
        int y_from = fromPos[1];
        delete[] fromPos;
        //send RunMove
        
        std::string send("<room roomId=\"");
        send.append(Fisher::cpRid);
        send.append("\">\n\t<data class=\"RunMove\" fromX=\"");
        zahl[0] = (char)('0' + x_from);
        send.append(zahl);
        send.append("\" fromY=\"");
        zahl[0] = (char)('0' + y_from);
        send.append(zahl);
        send.append("\" toX=\"");
        zahl[0] = (char)('0' + x_to);
        send.append(zahl);
        send.append("\" toY=\"");
        zahl[0] = (char)('0' + y_to);
        send.append(zahl);
        send.append("\"/>\n</room>");
        
        delete[] zahl;
        
        Ocean::Send(send);
    }
}

void Ocean::Close(){
    close(_socket);
}


/*******************************************************************Fisher*****/

#define searchCrumb(character) for(;text[pos] != character; ++pos){}            //Search the given character
#define searchAnotherCrumb(character) for(;text[pos] == character; ++pos){}     //Search another Character
#define sCrumbWhile(character) for(;text[pos] != character; ++pos)              //searchCrumb with while-body

namespace Fisher{
    char* cpRid = "";
    char* _color = "";
    
    byte _redPoints;
    byte _bluePoints;
    
    byte _redFields;
    byte _blueFields;
    
    bool _moveReqB = false;
    
    bool _fieldSet = false;
    
    Fisher::OnField _fieldEaten;
    Fisher::OnMove _moveEaten;
    Fisher::OnMoveReq _moveReq;
    
    char* _othersName = "";
    bool _othersNameIsSet = false;
}
/**
 * Set funcions to call for field, move and moveRequesion.
**/
void Fisher::SetDigestion(Fisher::OnField fieldEaten, Fisher::OnMove moveEaten, Fisher::OnMoveReq moveReq){
    _fieldEaten = fieldEaten;
    _moveEaten = moveEaten;
    _moveReq = moveReq;
}

void Fisher::Catch(char* text){
    int pos = 0;   
    int lng = strlen(text);   
    
    while(pos < lng){
        searchCrumb('<')
        pos++;
        
        if(text[pos] == 'b'){
            pos++;
// <blue displayName="N" color="C" points="P" fields="F"/> (XML-Doc 6.3)
            if(text[pos] == 'l'){                               
                pos = playerData(text, pos, false);
                
// <board> ... </board> (XML-Doc 6.4)
            }else{
                pos = fieldRel(text, pos);
            }
            
// for all importaint data-classes (XML-Doc 5, 6.1, 7)
        }else if(text[pos] == 'd'){
            pos += 12;

// +---- <data class="welcomeMessage" color="C"/>
            if(text[pos] == 'w'){
                pos = welcomeMsg(text, pos);
                
// +---- <data class="sc.framework.plugins.protocol.MoveRequest"/> (XML-Doc 7)
            }else if(text[pos] == 's' && text[pos + 1] == 'c'){
                pos = moveReq(text, pos);
            }
            
// <joined roomId="RID"/> (XML-Doc 4)
        }else if(text[pos] == 'j' && text[(pos += 4)] == 'e'){
            pos = joined(text, pos);
            
// <lastMove class="T" ZUG/> (XML-Doc 6.7)
        }else if(text[pos] == 'l'){                             
            pos = lastMove(text, pos);
            
// <red displayName="N" color="C" points="P" fields="F"/> (XML-Doc 6.3)
        }else if(text[pos] == 'r' && text[++pos] != 'o'){
            pos = playerData(text, pos, true);
            
// </protocol> (XML-Doc Teil III - 13)
        }else if(text[pos+1] == 'p' && text[pos] == '/'){
            endProtocol();
        }

        pos = nextHead(text, pos);
    }
    
    if(_moveReqB){
        _moveReq();
        _moveReqB = false;
    }
}

int Fisher::welcomeMsg(char* text, int pos){
    searchCrumb('r')

    searchCrumb('"')
    pos++;
    sCrumbWhile('"')
        _color = appendCharToCharArray(_color, text[pos]);
    
    return pos;
}

/**
 * XML 6.7 lastMove
 * 
 * @param text text to eat
 * @param pos position in text
 * @return position in text after runnung
 */
int Fisher::lastMove(char* text, int pos){
    searchCrumb(' ')
    searchCrumb('c')
    pos += 7;
    
    Move move = Move();
    
    if(text[pos] == 'S'){   //SetMove
        searchCrumb('"')
        ++pos;
        
        byte x = 0;
        byte y = 0;
        
        searchCrumb('"')
        ++pos;
        x = text[pos] - '0';
        pos += 4;
        
        searchCrumb('"')
        ++pos;
        y = text[pos] - '0';
        pos += 4;
        
        move.from = INVALID_POS;
        move.to = Tools::getPos(x, y);
    }else if(text[pos] == 'R'){   //RunMove
        searchCrumb('"')
        ++pos;
        
        byte fx = 0;
        byte fy = 0;
        
        byte tx = 0;
        byte ty = 0;
        
        searchCrumb('"')
        ++pos;
        fx = text[pos] - '0';
        pos += 4;
        
        searchCrumb('"')
        ++pos;
        fy = text[pos] - '0';
        pos += 4;
        
        searchCrumb('"')
        ++pos;
        tx = text[pos] - '0';
        pos += 4;
        
        searchCrumb('"')
        ++pos;
        ty = text[pos] - '0';
        pos += 4;
        
        move.from = Tools::getPos(fx, fy);
        move.to = Tools::getPos(tx, ty);
    }else if(text[pos] == 'N'){   //NullMove
        searchCrumb('"')
        ++pos;
        
        move.from = INVALID_POS;
        move.to = INVALID_POS;
    }
    
    searchCrumb('>')
    
    _moveEaten(move);
    
    return pos;
}

int Fisher::moveReq(char* text, int pos){
    _moveReqB = true;
    
    return pos;
}

/**
 * true = red
 * false = blue
 */
int Fisher::playerData(char* text, int pos, bool player){
    bool cont = true;
    bool points = false;
    bool fields = false;

    for(int runs = 0; cont && runs < 4 && (!points || !fields); runs++){
        for(;text[pos] != ' '; pos++){}
        pos++;

        if(text[pos] == 'p'){
            char* poiS = "";
            points = true;
            for(;text[pos] != '"'; pos++){}
            pos++;
            for(;text[pos] != '"'; pos++){
                poiS += text[pos];
            }

            if(player)
                _redPoints = atoi(poiS);
            else
                _bluePoints = atoi(poiS);
        }else if(text[pos] == 'f'){
            char* fieS = "";
            fields = true;
            for(;text[pos] != '"'; pos++){}
            pos++;
            for(;text[pos] != '"'; pos++)            {
                fieS += text[pos];
            }

            if(player)
                _redFields = atoi(fieS);
            else
                _blueFields = atoi(fieS);
        }else if(text[pos] == 'd'){
            char* disS = "\0";
            fields = true;
            for(;text[pos] != '"'; pos++){}
            pos++;
            for(;text[pos] != '"'; pos++){
                //disS += text[pos];
                disS = appendCharToCharArray(disS, text[pos]);
            }
            
            if(!_othersNameIsSet){
                if(player && _color[0] == 'b'){
                    _othersName = disS;
                    _othersNameIsSet = true;
                    
                    std::cout << "\n-----> Der aktuelle Gegner heißt: \"" << _othersName << "\"\n\n";
                }else if(!player && _color[0] == 'r'){
                    _othersName = disS;
                    _othersNameIsSet = true;
                    
                    std::cout << "\n-----> Der aktuelle Gegner heißt: \"" << _othersName << "\"\n\n";
                }
            }
        }else{
            for(;text[pos] != '"'; pos++){}
            pos++;
            for(;text[pos] != '"'; pos++){}
        }
    }
    return pos;
}

/**
 * Code is still running it, when field is set, because don't know, if code crashes without.
 * Find it out with test or when breadfish works.
 * 
 * @param text
 * @param pos
 * @return position after running fieldRel(char*, int)
 */
int Fisher::fieldRel(char* text, int pos){
    u_int64_t ones      = 0;
    u_int64_t twos      = 0;
    u_int64_t threes    = 0;
    
    for(;text[pos] != 's';pos++){}
    pos += 3;
    
    for(int x = 0; x < 8; x++){
      for(;text[pos] != '>';pos++){}
      pos++;

       for(int y = 0; y < 8; y++){
           searchCrumb('<')
           pos++;
           
           searchCrumb('"')
           pos++;

           byte val;
           val = text[pos] - '0';
           
           if(val == 0)continue;  //skip if field is empty

           bool owner = true;
           sCrumbWhile('>')
               if(text[pos] == '/')owner = false;
           
           if(owner){
               searchCrumb('p')
               searchCrumb('"')
               searchCrumb('"')
               searchCrumb('>')
               pos++;
               searchCrumb('>')
           }

           pos += 2;
           
           if(_fieldSet)continue;
           
           if(val == 1) ones |= ((uint64_t)1 << Tools::getPos(x, y));
           else if(val == 2) twos |= ((uint64_t)1 << Tools::getPos(x, y));
           else if(val == 3) threes |= ((uint64_t)1 << Tools::getPos(x, y));
           else std::cout << "----------" << (int)val << std::endl;
       }

       searchCrumb('>')
       pos++;
    }

    searchCrumb('b')
    pos++;
    
    if(_fieldSet)return pos;
    
    _fieldEaten(ones, twos, threes);
    _fieldSet = true;
    
    return pos;
}

int Fisher::joined(char* text, int pos){
    searchCrumb('"')
    pos++;
    cpRid = "";
    sCrumbWhile('"')
        cpRid = appendCharToCharArray(cpRid, text[pos]);
    
    return pos;
}

int Fisher::nextHead(char* text, int pos){
    for(; (pos < strlen(text) && text[pos] != '>'); pos++){}
    pos++;
    return pos;
}

char* Fisher::appendCharToCharArray(char* array, char a){
    size_t len = strlen(array);

    char* ret = new char[len+2];

    strcpy(ret, array);    
    ret[len] = a;
    ret[len+1] = '\0';

    return ret;
}

int Fisher::endProtocol(){
    Ocean::Close();
    Globals::_runningGame = false;
}