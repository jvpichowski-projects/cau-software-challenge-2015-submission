/* 
 * File:   SortedMoveMemory.cpp
 * Author: jonas
 * 
 * Created on 22. März 2015, 02:07
 */

#include "sortedMoveMemory.h"

#define SHELFSIZE 240 

namespace SortedMoveMemory
{
    bool _firstUse = true;
    ShelfStruct _shelf1[SHELFSIZE];
    
    int getShelfPos(Board board)
    {
        u_int64_t pos = board.used;
        pos ^= (board.mypos & board.oppos);
        pos ^= board.pointsdiff;
        //return (int)(pos & 255);  //faster to calc, but not better to save in memory
        return pos%SHELFSIZE;
    }
    
    int addSortedMoves(Board board, Move* moves,  int length)
    {
        int shelfPos = getShelfPos(board);
        bool searchPos = true;
        
        u_int32_t hash = board.pointsdiff;
        hash |= (board.movecount << 16);
        
        int usedFieldsStructPos = -1;
        UsedFieldsStruct1 usedFieldsStruct;
        if(searchPos && (usedFieldsStructPos = getUsedFieldsStructPos(board, shelfPos)) != -1){
            usedFieldsStruct = _shelf1[shelfPos].shelfStuff[usedFieldsStructPos];
        }
        else{
            searchPos = false;  //in this case, searching again is hopeless
            
            usedFieldsStruct = UsedFieldsStruct1();
            usedFieldsStruct.usedFiels = board.used;
            _shelf1[shelfPos].shelfStuff[_shelf1[shelfPos].length] = usedFieldsStruct;  //add
            //sort:
            for(int i = _shelf1[shelfPos].length; i > 0; i--){
                if(_shelf1[shelfPos].shelfStuff[i-1].usedFiels < _shelf1[shelfPos].shelfStuff[i].usedFiels)
                    break;
                UsedFieldsStruct1 tmp = _shelf1[shelfPos].shelfStuff[i-1];
                _shelf1[shelfPos].shelfStuff[i-1] = _shelf1[shelfPos].shelfStuff[i];
                _shelf1[shelfPos].shelfStuff[i] = tmp;
            }
            ++_shelf1[shelfPos].length; //update length
        }
        
        int opPoStructPos = -1;
        OpponentPositionsStruct2 opPoStruct;
        if(searchPos && (opPoStructPos = getOpponentPositionsStruct(board, shelfPos, usedFieldsStructPos)) != -1){
            opPoStruct = usedFieldsStruct.opponentPositions[opPoStructPos];
        }
        else{
            searchPos = false;  //in this case, searching again is hopeless
            opPoStruct = OpponentPositionsStruct2();
            opPoStruct.opponentPositions = board.oppos;
            usedFieldsStruct.opponentPositions[usedFieldsStruct.length] = opPoStruct;   //add
            //sort:
            for(int i = usedFieldsStruct.length; i > 0; i--){
                if(usedFieldsStruct.opponentPositions[i-1].opponentPositions < usedFieldsStruct.opponentPositions[i].opponentPositions)
                    break;
                OpponentPositionsStruct2 tmp = usedFieldsStruct.opponentPositions[i-1];
                usedFieldsStruct.opponentPositions[i-1] = usedFieldsStruct.opponentPositions[i];
                usedFieldsStruct.opponentPositions[i] = tmp;
            }
        }
        
        int pointsDeepStructPos = -1;
        PointsAndDeepStruct3 pointsDeepStruct;
        if(searchPos && (pointsDeepStructPos = getPointsDeepStruct(board, shelfPos, usedFieldsStructPos, opPoStructPos)) != -1){
            pointsDeepStruct = opPoStruct.pointsAndDeep[pointsDeepStructPos];
        }
        else{
            searchPos = false;  //in this case, searching again is hopeless
            pointsDeepStruct = PointsAndDeepStruct3();
            pointsDeepStruct.pointsAndDeepHash = hash;
            opPoStruct.pointsAndDeep[opPoStruct.length] = pointsDeepStruct; //add
            //sort:
            for(int i = opPoStruct.length; i > 0; i--){
                if(opPoStruct.pointsAndDeep[i-1].pointsAndDeepHash < opPoStruct.pointsAndDeep[i].pointsAndDeepHash)
                    break;
                PointsAndDeepStruct3 tmp = opPoStruct.pointsAndDeep[i-1];
                opPoStruct.pointsAndDeep[i-1] = opPoStruct.pointsAndDeep[i];
                opPoStruct.pointsAndDeep[i] = tmp;
            }
        }
        
        OwnPositionsStruct4 ownPosStruct = OwnPositionsStruct4();
        ownPosStruct.ownPositions = board.mypos;
        ownPosStruct.sortedmoves = moves;   //add the information
        ownPosStruct.length = length;       //
        pointsDeepStruct.ownPositions[pointsDeepStruct.length] = ownPosStruct;  //add
        //sort:
        for(int i = 0; i > 0; i--){
            if(pointsDeepStruct.ownPositions[i-1].ownPositions < pointsDeepStruct.ownPositions[i].ownPositions)
                break;
            OwnPositionsStruct4 tmp = pointsDeepStruct.ownPositions[i-1];
            pointsDeepStruct.ownPositions[i-1] = pointsDeepStruct.ownPositions[i];
            pointsDeepStruct.ownPositions[i] = tmp;
        }
    }
    
    Move* getSortedMoves(Board board, bool* inMemory, int* length)
    {
        Move* result;
        
        
        int shelfPos = getShelfPos(board);
        if(shelfPos == -1){
            *inMemory = false;
            result[0] = Move();
            *length = -1;
        }
        
        int usedFieldsStructPos = getUsedFieldsStructPos(board, shelfPos);
        if(usedFieldsStructPos == -1){
            *inMemory = false;
            result[0] = Move();
            *length = -1;
        }
        
        int opPostructPos = getOpponentPositionsStruct(board, shelfPos, usedFieldsStructPos);
        if(opPostructPos == -1){
            *inMemory = false;
            result[0] = Move();
            *length = -1;
        }
        
        int pointsDeepStructPos = getPointsDeepStruct(board, shelfPos, usedFieldsStructPos, opPostructPos);
        if(pointsDeepStructPos == -1){
            *inMemory = false;
            result[0] = Move();
            *length = -1;
        }
        
        int ownPosStructPos = getOwnPostruct(board, shelfPos, usedFieldsStructPos, opPostructPos, pointsDeepStructPos);
        if(ownPosStructPos == -1){
            *inMemory = false;
            result[0] = Move();
            *length = -1;
        }
        
        
        *inMemory = true;
        *length = _shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[ownPosStructPos].length;
        return _shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[ownPosStructPos].sortedmoves;
    }
    
    int getOwnPostruct(Board board, int shelfPos, int usedFieldsStructPos, int opPostructPos, int pointsDeepStructPos)
    {
        if(_shelf1[shelfPos].length == 0)
            return -1;
        
        int min = 0;
        int max = _shelf1[shelfPos].length;
        int posnow;
        
        while(min < max)
        {
            int z = (max - min);

            posnow = min + (z/2) + (z & 1);

            if(max == posnow)
            {
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[posnow-1].ownPositions == board.mypos)
                            return posnow - 1;
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[posnow].ownPositions == board.mypos)
                            return posnow;
                    else
                            return -1;
            }

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[posnow].ownPositions > board.mypos)
                    max = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[posnow].ownPositions < board.mypos)
                    min = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[pointsDeepStructPos].ownPositions[posnow].ownPositions == board.mypos)
                    return posnow;
        }

        return -1;
    }
    
    int getPointsDeepStruct(Board board, int shelfPos, int usedFieldsStructPos, int opPostructPos)
    {
        if(_shelf1[shelfPos].length == 0)
            return -1;
        
        int min = 0;
        int max = _shelf1[shelfPos].length;
        int posnow;
        
        u_int32_t search = board.pointsdiff;
        search |= (board.movecount << 16);
        
        while(min < max)
        {
            int z = (max - min);

            posnow = min + (z/2) + (z & 1);

            if(max == posnow)
            {
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[posnow-1].pointsAndDeepHash == search)
                            return posnow - 1;
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[posnow].pointsAndDeepHash == search)
                            return posnow;
                    else
                            return -1;
            }

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[posnow].pointsAndDeepHash > search)
                    max = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[posnow].pointsAndDeepHash < search)
                    min = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[opPostructPos].pointsAndDeep[posnow].pointsAndDeepHash == search)
                    return posnow;
        }

        return -1;
    }
    
    int getOpponentPositionsStruct(Board board, int shelfPos, int usedFieldsStructPos)
    {
        if(_shelf1[shelfPos].length == 0)
            return -1;
        
        int min = 0;
        int max = _shelf1[shelfPos].length;
        int posnow;
        
        while(min < max)
        {
            int z = (max - min);

            posnow = min + (z/2) + (z & 1);

            if(max == posnow)
            {
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[posnow-1].opponentPositions == board.oppos)
                            return posnow - 1;
                    if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[posnow].opponentPositions == board.oppos)
                            return posnow;
                    else
                            return -1;
            }

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[posnow].opponentPositions > board.oppos)
                    max = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[posnow].opponentPositions < board.oppos)
                    min = posnow;

            if(_shelf1[shelfPos].shelfStuff[usedFieldsStructPos].opponentPositions[posnow].opponentPositions == board.oppos)
                    return posnow;
        }

        return -1;
    }
    
    int getUsedFieldsStructPos(Board board, int shelfPos)
    {
        if(_shelf1[shelfPos].length == 0)
            return -1;
        
        int min = 0;
        int max = _shelf1[shelfPos].length;
        int posnow;
        
        while(min < max)
        {
            int z = (max - min);

            posnow = min + (z/2) + (z & 1);

            if(max == posnow)
            {
                    if(_shelf1[shelfPos].shelfStuff[posnow-1].usedFiels == board.used)
                            return posnow - 1;
                    if(_shelf1[shelfPos].shelfStuff[posnow].usedFiels == board.used)
                            return posnow;
                    else
                            return -1;
            }

            if(_shelf1[shelfPos].shelfStuff[posnow].usedFiels > board.used)
                    max = posnow;

            if(_shelf1[shelfPos].shelfStuff[posnow].usedFiels < board.used)
                    min = posnow;

            if(_shelf1[shelfPos].shelfStuff[posnow].usedFiels == board.used)
                    return posnow;
        }

        return -1;
    }
    
    int init()
    {   
        //bad long code to clean memory
        if(!_firstUse){
            for(int s1 = 0; s1 < SHELFSIZE; s1++){
                for(int u2 = 0; u2 < _shelf1[s1].length; u2++){
                    for(int o3 = 0; o3 < _shelf1[s1].shelfStuff[u2].length; o3++){                    
                        for(int p4 = 0; p4 < _shelf1[s1].shelfStuff[u2].opponentPositions[o3].length; p4++){                            
                            for(int o5 = 0; o5 < _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep[p4].length; o5++){                    
                                _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep[p4].ownPositions[o5].length = 0;
                                delete[] _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep[p4].ownPositions[o5].sortedmoves;                    
                            }                    
                            _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep[p4].length = 0;
                            delete[] _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep[p4].ownPositions;
                        }                    
                        _shelf1[s1].shelfStuff[u2].opponentPositions[o3].length = 0;
                        delete[] _shelf1[s1].shelfStuff[u2].opponentPositions[o3].pointsAndDeep;
                    }                    
                    delete[] _shelf1[s1].shelfStuff[u2].opponentPositions;
                    _shelf1[s1].shelfStuff[u2].length = 0;
                }         
                _shelf1[s1].length = 0;
            }
        }
        else
        {
            _firstUse = false;
            for(int i = 0; i < SHELFSIZE; i++){
                _shelf1[i] = ShelfStruct();
            }
        }
    }
    
    
}
