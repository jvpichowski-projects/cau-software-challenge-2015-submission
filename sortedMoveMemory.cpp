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
        
        OwnPositionsStruct4 sortedMovesS = OwnPositionsStruct4();
        sortedMovesS.length = length;
        sortedMovesS.sortedmoves = moves;
        sortedMovesS.ownPositions = board.mypos;
        
        int usedFieldsStructPos = getUsedFieldsStructPos(board, shelfPos);
        if(usedFieldsStructPos == -1){
            int hash = board.pointsdiff;
            hash |= (board.movecount << 16);
            
            PointsAndDeepStruct3 pointsDeepS = PointsAndDeepStruct3();
            pointsDeepS.length = 1;
            pointsDeepS.ownPositions[0] = sortedMovesS;
            pointsDeepS.pointsAndDeepHash = hash;
            
            OpponentPositionsStruct2 opPosS = OpponentPositionsStruct2();
            opPosS.length = 1;
            opPosS.opponentPositions = board.oppos;
            opPosS.pointsAndDeep[0] = pointsDeepS;
            
            
            
        }
        
        int opPostructPos = getOpponentPositionsStruct(board, shelfPos, usedFieldsStructPos);
        if(opPostructPos == -1){
            int hash = board.pointsdiff;
            hash |= (board.movecount << 16);
            
            PointsAndDeepStruct3 pointsDeepS = PointsAndDeepStruct3();
            pointsDeepS.length = 1;
            pointsDeepS.ownPositions[0] = sortedMovesS;
            pointsDeepS.pointsAndDeepHash = hash;
            
            OpponentPositionsStruct2 opPosS = OpponentPositionsStruct2();
            opPosS.length = 1;
            opPosS.opponentPositions = board.oppos;
            opPosS.pointsAndDeep[0] = pointsDeepS;
        }
        
        int pointsDeepStructPos = getPointsDeepStruct(board, shelfPos, usedFieldsStructPos, opPostructPos);
        if(pointsDeepStructPos == -1){
            int hash = board.pointsdiff;
            hash |= (board.movecount << 16);
            
            PointsAndDeepStruct3 pointsDeepS = PointsAndDeepStruct3();
            pointsDeepS.length = 1;
            pointsDeepS.ownPositions[0] = sortedMovesS;
            pointsDeepS.pointsAndDeepHash = hash;
        }
        
        int ownPosStructPos = getOwnPostruct(board, shelfPos, usedFieldsStructPos, opPostructPos, pointsDeepStructPos);
        if(ownPosStructPos == -1){
            
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
        
        int search = board.pointsdiff;
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
