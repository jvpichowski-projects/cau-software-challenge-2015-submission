/* 
 * File:   SortedMoveMemory.cpp
 * Author: jonas
 * 
 * Created on 22. März 2015, 02:07
 */

#include "sortedMoveMemory.h"

namespace SortedMoveMemory
{
    bool _firstUse = true;
    ShelfStruct _shelf1[240];
    
    int getShelfPos(Board board)
    {
        u_int64_t pos = board.used;
        pos ^= (board.mypos & board.oppos);
        pos ^= board.pointsdiff;
        //return (int)(pos & 255);  //faster to calc, but not better to save in memory
        return pos%240;
    }
    
    Move* getSortedMoves(Board board, bool* inMemory)
    {
        Move* result;
        inMemory = true;
        
        int shelfPos = getShelfPos(board);
        int UsedFieldsStructPos = getUsedFieldsStructPos(board, shelfPos);
        
    }
    
    int getUsedFieldsStructPos(Board board, int shelfPos)
    {
        if(_shelf1[shelfPos].length == 0)
            return -1;
        
        int min = 0;
        int max = _shelf1[shelfPos];
        
        while(min != max)
        {
            
        }
    }
    
    int init()
    {   
        //bad long code to clean memory
        if(!_firstUse){
            for(int s1 = 0; s1 < 240; s1++){
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
            for(int i = 0; i < 240; i++){
                _shelf1[i] = ShelfStruct();
            }
        }
    }
    
    
}
