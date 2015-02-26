#include "tt.h"
#include "tools.h"
#include "boardstate.h"

#ifdef tt

namespace TT{

    u_int64_t lookup_count;
    u_int64_t positiv_lookup_count;
    u_int64_t collision_count;
    
    Entry* EntryMap = new Entry[ENTRY_MAP_SIZE];

    u_int64_t getHash(Board board){
        u_int64_t hash = 0;
        int length = 0;
        int *used = Tools::fastBitScan(board.used, &length);
        for(int i= 0; i < length; ++i){
            hash ^= prns[used[i]];
        }
        delete[] used;
        int *mypos = Tools::fastBitScan(board.mypos, &length);
        for(int i = 0; i < length; ++i){
            hash ^= prns[60+mypos[i]];
        }
        delete[] mypos;
        int *oppos = Tools::fastBitScan(board.oppos, &length);
        for(int i = 0; i < length; ++i){
            hash ^= prns[120+oppos[i]];
        }
        delete[] oppos;
        
        return hash ^ prns[180+board.movecount] ^ prns[/*307+64*/371+board.pointsdiff] ^ prns[434+board.turn];
    }
    void storeEntry(Board board, int depth, int type,  int value, Move bestMove){ 
        u_int64_t hash = getHash(board);
        Entry lastEntry = EntryMap[hash % ENTRY_MAP_SIZE];
#ifdef secure_tt
        if(lastEntry.board != Board() && lastEntry.depth >= depth){
            return;
        }
#else
        if(lastEntry.hash != 0 && lastEntry.depth >= depth){
            return;
        }
#endif
        
        Entry entry = Entry();
        
        entry.best = bestMove;
        entry.score = value;
        entry.depth = depth;
        entry.type = type;
        
#ifdef secure_tt
        entry.board = board; 
#else
        entry.hash = hash;  
#endif
        
        EntryMap[hash % ENTRY_MAP_SIZE] = entry;
    }
    
    bool getEntry(Board board, int depth, int *type,  int *value, Move *bestMove){
        u_int64_t hash = getHash(board);
        Entry entry = EntryMap[hash % ENTRY_MAP_SIZE];
        ++lookup_count;
#ifdef secure_tt
        if(entry.board != board || entry.depth < depth){
            return false;
        }
#else
        if(entry.hash != hash || entry.depth < depth){
            return false;
        }
#endif
        ++positiv_lookup_count;
        
        *type = entry.type;
        *value = entry.score;
        *bestMove = entry.best;
        return true;
    }
    
};

#endif