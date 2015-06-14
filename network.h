/* 
 * File:   network.h
 *
 * Created on 15. Februar 2015, 20:34
 */

#ifndef NETWORK_H
#define	NETWORK_H

#include <fcntl.h>
#include <sys/socket.h> 
#include <netdb.h> 
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "types.h"
#include "globals.h"
#include "tools.h"

namespace Fisher{
    typedef void (*OnField)(u_int64_t ones, u_int64_t twos, u_int64_t threes); 
    typedef void (*OnMove)(Move move);
    typedef void (*OnMoveReq)();
    extern char* cpRid;
    extern char* _color;
    extern bool _moveReqB;
    extern bool _moveReqB;
    extern bool _fieldSet;
    void Catch(char* text);
    void SetDigestion(Fisher::OnField fieldEaten, Fisher::OnMove moveEaten, Fisher::OnMoveReq moveReq);
    int welcomeMsg(char* text, int pos);
    int moveReq(char* text, int pos);
    int nextHead(char* text, int pos);
    int joined(char* text, int pos);
    int fieldRel(char* text, int pos);
    int lastMove(char* text, int pos);
    int playerData(char* text, int pos, bool player);
    char* appendCharToCharArray(char* array, char a);
    int endProtocol();
}

namespace Ocean{
    typedef void (*OnReceive)(char* msg);
    typedef void (*OnTick)(); 
    extern int _socket;
    extern std::string packetBuffer;
    extern Ocean::OnTick _ont;
    extern Ocean::OnReceive _onr;
    void SwimTo(char* ip, char* port, OnReceive onr, OnTick ont);
    void Send(char* msg);
    void GetFood();
    void Send(std::string msg);
    void Send(Move move);
    void Close();
}

#endif	/* NETWORK_H */

