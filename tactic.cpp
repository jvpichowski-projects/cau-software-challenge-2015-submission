/* 
 * File:   tactic.cpp
 * Author: jonas
 * 
 * Created on 7. März 2015, 00:08
 */
#include "tactic.h"



namespace Tactic
{
    int64_t _fieldsAround[60] = {
            /* |  00 */0x182ULL,             /* |  01 */0x305ULL,             /* |  02 */0x60AULL,             /* |  03 */0xC14ULL,
            /* |  04 */0x1828ULL,            /* |  05 */0x3050ULL,            /* |  06 */0x6020ULL,            /* |  07 */0x8101ULL,
            /* |  08 */0x18283ULL,           /* |  09 */0x30506ULL,           /* |  10 */0x60A0CULL,           /* |  11 */0xC1418ULL,
            /* |  12 */0x182830ULL,          /* |  13 */0x305060ULL,          /* |  14 */0x202040ULL,          /* |  15 */0xC10180ULL,
            /* |  16 */0x1828300ULL,         /* |  17 */0x3050600ULL,         /* |  18 */0x60A0C00ULL,         /* |  19 */0xC141800ULL,
            /* |  20 */0x18283000ULL,        /* |  21 */0x30106000ULL,        /* |  22 */0x40808000ULL,        /* |  23 */0xC1418000ULL,
            /* |  24 */0x182830000ULL,       /* |  25 */0x305060000ULL,       /* |  26 */0x60A0C0000ULL,       /* |  27 */0xC14180000ULL,
            /* |  28 */0x1828300000ULL,      /* |  29 */0x1010200000ULL,      /* |  30 */0x6080C00000ULL,      /* |  31 */0xC141800000ULL,
            /* |  32 */0x18283000000ULL,     /* |  33 */0x30506000000ULL,     /* |  34 */0x60A0C000000ULL,     /* |  35 */0xC1418000000ULL,
            /* |  36 */0x180830000000ULL,    /* |  37 */0x204040000000ULL,    /* |  38 */0x60A0C0000000ULL,    /* |  39 */0xC14180000000ULL,
            /* |  40 */0x1828300000000ULL,   /* |  41 */0x3050600000000ULL,   /* |  42 */0x60A0C00000000ULL,   /* |  43 */0xC141800000000ULL,
            /* |  44 */0x8081000000000ULL,   /* |  45 */0x30406000000000ULL,  /* |  46 */0x60A0C000000000ULL,  /* |  47 */0xC1418000000000ULL,
            /* |  48 */0x182830000000000ULL, /* |  49 */0x305060000000000ULL, /* |  50 */0x60A0C0000000000ULL, /* |  51 */0xC04180000000000ULL,
            /* |  52 */0x20200000000000ULL,  /* |  53 */0x50600000000000ULL,  /* |  54 */0xA0C00000000000ULL,  /* |  55 */0x141800000000000ULL,
            /* |  56 */0x283000000000000ULL, /* |  57 */0x506000000000000ULL, /* |  58 */0xA0C000000000000ULL, /* |  59 */0x408000000000000ULL   };

    int64_t _fieldsAroundAround[60] = {
            /* |  00 */0x18204ULL,           /* |  01 */0x38488ULL,           /* |  02 */0x70911ULL,           /* |  03 */0xE1222ULL,
            /* |  04 */0x1C2444ULL,          /* |  05 */0x384808ULL,          /* |  06 */0x301010ULL,          /* |  07 */0xC10202ULL,
            /* |  08 */0x1C20404ULL,         /* |  09 */0x3848889ULL,         /* |  10 */0x7091112ULL,         /* |  11 */0xE122224ULL,
            /* |  12 */0x1C244448ULL,        /* |  13 */0x38080810ULL,        /* |  14 */0x30101020ULL,        /* |  15 */0xC1020203ULL,
            /* |  16 */0x1C2440487ULL,       /* |  17 */0x38488890EULL,       /* |  18 */0x70911121CULL,       /* |  19 */0xE12222438ULL,
            /* |  20 */0x1C24044870ULL,      /* |  21 */0x1808081060ULL,      /* |  22 */0x6081010180ULL,      /* |  23 */0xE102020380ULL,
            /* |  24 */0x1C244448700ULL,     /* |  25 */0x38488890E00ULL,     /* |  26 */0x70911121C00ULL,     /* |  27 */0xE1222243800ULL,
            /* |  28 */0x1C0404087000ULL,    /* |  29 */0x180808106000ULL,    /* |  30 */0x608101018000ULL,    /* |  31 */0xE12202438000ULL,
            /* |  32 */0x1C24444870000ULL,   /* |  33 */0x38488890E0000ULL,   /* |  34 */0x70911121C0000ULL,   /* |  35 */0xE120224380000ULL,
            /* |  36 */0xC040408300000ULL,   /* |  37 */0x30408080C00000ULL,  /* |  38 */0x70810101C00000ULL,  /* |  39 */0xE1222243800000ULL,
            /* |  40 */0x1C2444487000000ULL, /* |  41 */0x38488890E000000ULL, /* |  42 */0x70911121C000000ULL, /* |  43 */0xE02020438000000ULL,
            /* |  44 */0xC04040830000000ULL, /* |  45 */0x408080C0000000ULL,  /* |  46 */0x910121C0000000ULL,  /* |  47 */0x122224380000000ULL,
            /* |  48 */0x244448700000000ULL, /* |  49 */0x488890E00000000ULL, /* |  50 */0x901121C00000000ULL, /* |  51 */0x202041800000000ULL,
            /* |  52 */0x40406000000000ULL,  /* |  53 */0x8080E000000000ULL,  /* |  54 */0x11121C000000000ULL, /* |  55 */0x222438000000000ULL,
            /* |  56 */0x444870000000000ULL, /* |  57 */0x8890E0000000000ULL, /* |  58 */0x1021C0000000000ULL, /* |  59 */0x204180000000000ULL};
    
    int _myPosLng;
    int* _myPos;
    int _opPosLng;
    int* _opPos;
    
    int _newDiff;
    int _nowDiff;
    
    Move _newMove;
    
    /**
     * 
     * @param resultMoves
     * @param resultmoveLng
     * @param activePlayer
     * @return 0 = no result, continue the normal way | 1 = continue calculating with the active players | 2 = 1 + continue with resultMoves | 3 = take first Move to send
     */
    int preliminaries(Move* resultMoves, int* resultmoveLng, u_int64_t* activePlayer)
    {
        _newMove.from = INVALID_POS;
        _newMove.to = INVALID_POS;
        delete[] _myPos, _opPos;
        _myPosLng;
        _myPos = Tools::bitScan(Globals::_board.mypos, &_myPosLng);
        _opPosLng;
        _opPos = Tools::bitScan(Globals::_board.oppos, &_opPosLng);
        
        _nowDiff = getAvailablePoints(ID_WE, Globals::_board) - getAvailablePoints(ID_OPPONENT, Globals::_board);
        _newDiff = _nowDiff;
        
        getOneFieldMove();
        
        if((_newDiff - _nowDiff) > 5)
        {
            resultMoves = &_newMove;
            return 1;
        }
    }
    
    int getOneFieldMove()
    {
        int length = 0;
        for(u_int8_t i = 0; i < 60; i++){
            int* pos = Tools::getPos(i);            
            u_int64_t filter;
            u_int64_t searched;            
            if(pos[0] == 0){
                filter = (3ULL << i);
                searched = (1 << i);
            }else if(pos[0] == (6 + (pos[1] % 2))){
                filter = (3ULL << (i-1));
                searched = (2 << (i-1));
            }else{
                filter = (7ULL << (i-1));
                searched = (2 << (i-1));}
            if(searched == (filter & ~Globals::_board.used))
                length++;
        }
        int* posis = new int[length];
        int pos_i = 0;
        for(u_int8_t i = 0; i < 60; i++){
            int* pos = Tools::getPos(i);
            u_int64_t filter;
            u_int64_t searched;
            if(pos[0] == 0){
                filter = (3ULL << i);
                searched = (1 << i);
            }else if(pos[0] == (6 + (pos[1] % 2))){
                filter = (3ULL << (i-1));
                searched = (2 << (i-1));
            }else{
                filter = (7ULL << (i-1));
                searched = (2 << (i-1));}
            if(searched == (filter & ~Globals::_board.used)){
                posis[pos_i] = i;
                pos_i++;}
        }
        /////////////////////////////
        
        for(u_int8_t i = 0; i < length; i++)
        {
            selectOneFieldMove(posis[i]);
        }
    }
    
    int selectOneFieldMove(int pos)
    {
        for(u_int8_t i = 0; i < _myPosLng; i++)
        {
            if((Tools::getMoveField(_myPos[i], Globals::_board.used) & (1 << pos)) == (1 << pos))
            {
                Move move;
                move.from = _myPos[i];
                move.to = pos;
                
                if(!BoardTools::isValidMove(Globals::_board, move, ID_WE)){
                    printf("\n\ntttttttttttttttoools: Move Invalid!!!!!");
                    continue;
                }
                
                Board futBoard = Globals::_board;
                BoardTools::apply(&futBoard, ID_WE, move);
                
                int diff = getAvailablePointsDiff(futBoard);
                
                if(diff > _newDiff){
                    _newDiff = diff;
                    _newMove = move;
                }
            }
        }
    }
    
    int getAvailablePointsDiff(Board board)
    {
        int ruckgabe = 0;
        
        ruckgabe += getAvailablePoints(ID_WE, board);
        
        ruckgabe -= getAvailablePoints(ID_OPPONENT, board);
        
        return ruckgabe;
    }
    
    int getAvailablePoints(u_int8_t player, Board board)
    {
        u_int64_t playField;
        if(player == ID_WE)
            playField = board.mypos;
        else
            playField = board.oppos;
        
        int playLng;
        int* playPos = Tools::bitScan(playField, &playLng);
        int ruckgabe = 0;
        
        u_int64_t avFields = 0ULL;
        
        for(u_int8_t i = 0; i < playLng; i++)
        {
        
            avFields |= getBigMoveField(playPos[i], board);
        
        
        }
        
        ruckgabe += Tools::popCount(avFields & Globals::ones);
        ruckgabe += Tools::popCount(avFields & Globals::twos)*2;
        ruckgabe += Tools::popCount(avFields & Globals::threes)*3;
        
        
        return ruckgabe;
    }
    
    u_int64_t getBigMoveField(u_int8_t pos, Board board)
    {
        u_int64_t result = Tools::getMoveField(pos, board.used);
        
        int len;
        int* posis = Tools::bitScan(result, &len);
        
        for(u_int8_t i = 0; i < len; i++)
            result |= posis[i];
        
        /*u_int64_t ring = result & _fieldsAround[pos];
        int length;
        
        int* ringpos = Tools::bitScan(ring, &length);        
        for(u_int8_t i = 0; i < length; i++)
        {
            result |= Tools::getMoveField(ringpos[i], board.used);
        }
        
        delete[] ringpos;
        
        ring = result & _fieldsAroundAround[pos];        
        ringpos = Tools::bitScan(ring, &length);
        for(u_int8_t i = 0; i < length; i++)
        {
            int mflng;
            u_int64_t mf = Tools::getMoveField(ringpos[i], board.used);
            result |= mf;
            
            int* mfPosis = Tools::bitScan(mf, &mflng);
            
            for(u_int8_t j = 0; j < mflng; j++)
            {
                result |= Tools::getMoveField(mfPosis[j], board.used);
            }
        }*/
        
        return result;
    }
}