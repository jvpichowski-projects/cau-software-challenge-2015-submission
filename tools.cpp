#include "tools.h"
/* 
 * File:   main.cpp
 * Author: jan
 *
 * Created on 12. Januar 2015, 17:07
 */

using namespace std;

int ms1bTable[]= {
        0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 7, 7, 3};

namespace Tools
{
    
    /**
    * bitScanReverse
    * @author Eugene Nalimov
    * @param bb bitboard to scan
    * @return index (0..63) of most significant one bit
    */
    int bitScanReverse(u_int64_t bb)
    {
        int result = 0;
        if (bb > 0xFFFFFFFF)
        {
            bb >>= 32;
            result = 32;
        }
        if (bb > 0xFFFF)
        {
            bb >>= 16;
            result += 16;
        }
        if (bb > 0xFF)
        {
            bb >>= 8;
            result += 8;
        }
        return result + ms1bTable[bb];
    }
    
    
    u_int64_t set(u_int64_t field, int pos){
        return field | 1LL << pos;
    }

    u_int64_t clear(u_int64_t field, int pos){
        return field & ~(1LL<<pos);
    }

    bool isSet(u_int64_t field, int pos){
        return ((field >> pos) & 1LL ) == 1;
    }

    void printField(u_int64_t field){
         cout << "Field:" << endl;
         cout << " ";
        for(int i = 0; i <= 63; i++){
            if(i == 7 || i == 22 || i == 37 ||  i == 52 ){
                cout << endl;
            }else if(i == 15 || i == 30 ||  i == 45 ||  i == 60){
                cout << endl << " ";
            }
            cout << " ";
            cout << isSet(field, i);
        }
         cout << endl;
    }
//    void printBoard(Board *board){
//         cout << "Board:" << endl;
//         cout << " ";
//        for(int i = 0; i <= 63; i++){
//            if(i == 7 || i == 22 || i == 37 ||  i == 52 ){
//                cout << endl;
//            }else if(i == 15 || i == 30 ||  i == 45 ||  i == 60){
//                cout << endl << " ";
//            }
//            cout << " ";
//            if(isSet(board->mPositions[ID_WE], i)){
//                cout << "W";
//            }else if(isSet(board->mPositions[!ID_WE], i)){
//                cout << "O";
//            }else if(isSet(board->mUsed, i)){
//                cout << "#";
//            }else if(isSet(Globals::ones, i)){
//                cout << 1;
//            }else if(isSet(Globals::twos, i)){
//                cout << 2;
//            }else if(isSet(Globals::threes, i)){
//                cout << 3;
//            }
//            
//        }
//         cout << endl;
//    }

//--------------------------------------------------------------------------move
    
    u_int64_t moveField(u_int64_t field, int dir, int pos){
        return ( field | 1LL << (pos+dir) ) & ~(1LL<<pos);
    }

    int movePos(int pos, int dir){
        return pos+dir;
    }

    u_int64_t left(u_int64_t field, int pos){
        return ( field | 1LL << (pos-1) ) & ~(1LL<<pos);
    }

    u_int64_t right(u_int64_t field, int pos){
        return ( field | 1LL << (pos+1) ) & ~(1LL<<pos);
    }

    u_int64_t topLeft(u_int64_t field, int pos){
        return ( field | 1LL << (pos-8) ) & ~(1LL<<pos);
    }

    u_int64_t topRight(u_int64_t field, int pos){
        return ( field | 1LL << (pos-7) ) & ~(1<<pos);
    }

    u_int64_t bottomLeft(u_int64_t field, int pos){
        return ( field | 1LL << (pos+7) ) & ~(1LL<<pos);
    }

    u_int64_t bottomRight(u_int64_t field, int pos){
        return ( field | 1LL << (pos+8) ) & ~(1LL<<pos);
    }

    u_int64_t initEmptyFields(){        
        return (1LL << 63) | (1LL << 62) | (1LL << 61) | (1LL << 60);
    }
    
    u_int64_t getMoveField(int pos, u_int64_t used){
        
        //------------------------------top left to down right------------------
        
        int right_shift = 59-pos;
        
        u_int64_t tl_dr = (ROW_TL_DR << pos) | (ROW_TL_DR_LAST >> right_shift);
        
        int l = 0;
        int *hits = Tools::bitScan(tl_dr & STOP_TOP_LEFT, &l);
        for(int i = 0; i < l; ++i){
            if(*hits <= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tl_dr = tl_dr >> *hits;
                tl_dr = tl_dr << *hits;
            }
            ++hits;
        }
        hits-=l;
        delete[] hits;
        
        hits = Tools::bitScan(tl_dr & STOP_DOWN_RIGHT, &l);
        int i;
        for(i = 0; i < l; ++i){
            if(*hits >= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tl_dr = tl_dr << 63-*hits;
                tl_dr = tl_dr >> 63-*hits;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        hits = Tools::bitScan(tl_dr & used, &l);
        for(i = 0; i < l; ++i){
            if(*hits < pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tl_dr = tl_dr >> *hits+1;
                tl_dr = tl_dr << *hits+1;
            }else if(*hits > pos){
                tl_dr = tl_dr << 63-*hits+1;
                tl_dr = tl_dr >> 63-*hits+1;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        //----------------------top right to down left--------------------------
        
        u_int64_t tr_dl = (ROW_TR_DL << pos) | (ROW_TR_DL_LAST >> right_shift);
        hits = Tools::bitScan(tr_dl & STOP_TOP_RIGHT, &l);
        for(int i = 0; i < l; ++i){
            if(*hits <= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tr_dl = tr_dl >> *hits;
                tr_dl = tr_dl << *hits;
            }
            ++hits;
        }
        hits-=l;
        delete[] hits;
        
        hits = Tools::bitScan(tr_dl & STOP_DOWN_LEFT, &l);
        for(i = 0; i < l; ++i){
            if(*hits >= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tr_dl = tr_dl << 63-*hits;
                tr_dl = tr_dl >> 63-*hits;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        hits = Tools::bitScan(tr_dl & used, &l);
        for(i = 0; i < l; ++i){
            if(*hits < pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                tr_dl = tr_dl >> *hits+1;
                tr_dl = tr_dl << *hits+1;
            }else if(*hits > pos){
                tr_dl = tr_dl << 63-*hits+1;
                tr_dl = tr_dl >> 63-*hits+1;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        //----------------------left to right-----------------------------------
        
        u_int64_t l_r = (ROW_LR << pos) | (ROW_LR_LAST >> right_shift);
        hits = Tools::bitScan(l_r & STOP_LEFT, &l);
        for(int i = 0; i < l; ++i){
            if(*hits <= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                l_r = l_r >> *hits;
                l_r = l_r << *hits;
            }
            ++hits;
        }
        hits-=l;
        delete[] hits;
        
        hits = Tools::bitScan(l_r & STOP_RIGHT, &l);
        for(i = 0; i < l; ++i){
            if(*hits >= pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                l_r = l_r << 63-*hits;
                l_r = l_r >> 63-*hits;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        hits = Tools::bitScan(l_r & used, &l);
        for(i = 0; i < l; ++i){
            if(*hits < pos){
                //cout << "Pos " << *hits << " strtPos " << pos << endl;
                l_r = l_r >> *hits+1;
                l_r = l_r << *hits+1;
            }else if(*hits > pos){
                l_r = l_r << 63-*hits+1;
                l_r = l_r >> 63-*hits+1;
                //always from low to heigh, all next hits are behind this
                break;
            }
            ++hits;
        }
        hits-=i;
        delete[] hits;
        
        //------------------------combine lines---------------------------------
        
        u_int64_t result = tl_dr | tr_dl | l_r;
        result &= ~FIT;
        result &= ~used;
        
        result &= ~(1LL << pos);          //No moves to own position anymore.
        
        //printField(result);
        
        return result;
    }
    
//----------------------------------------------------------------------bittools

    const int index64[64] = {
        0, 47,  1, 56, 48, 27,  2, 60,
       57, 49, 41, 37, 28, 16,  3, 61,
       54, 58, 35, 52, 50, 42, 21, 44,
       38, 32, 29, 23, 17, 11,  4, 62,
       46, 55, 26, 59, 40, 36, 15, 53,
       34, 51, 20, 43, 31, 22, 10, 45,
       25, 39, 14, 33, 19, 30,  9, 24,
       13, 18,  8, 12,  7,  6,  5, 63
    };

    /**
     * bitScanForward
     * @author Kim Walisch (2012)
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of least significant one bit
     */
    int bitScanForward(u_int64_t bb) {
       const u_int64_t debruijn64 = 0x03f79d71b4cb0a89;
       //assert (bb != 0);
       return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
    }
    
    

    const u_int64_t k1 = 0x5555555555555555; /*  -1/3   */
    const u_int64_t k2 = 0x3333333333333333; /*  -1/5   */
    const u_int64_t k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
    const u_int64_t kf = 0x0101010101010101; /*  -1/255 */

    int popCount (u_int64_t x) {
        x =  x       - ((x >> 1)  & k1); /* put count of each 2 bits into those 2 bits */
        x = (x & k2) + ((x >> 2)  & k2); /* put count of each 4 bits into those 4 bits */
        x = (x       +  (x >> 4)) & k4 ; /* put count of each 8 bits into those 8 bits */
        x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
        return (int) x;
    }
    
    /**
     * Only fast with sparsley populated 1 bits
     * 
     * @param x
     * @return 
     */
    int fastPopCount (u_int64_t x) {
        int count = 0;
        while (x) {
            count++;
            x &= x - 1; // reset LS1B
        }
        return count;
    }
    
    
    int* fastBitScan (u_int64_t x, int *length){
        *length = fastPopCount(x);
        int *result = new int[*length];
        for(int i = 0; i < *length; ++i){
            *result = bitScanForward(x);
            x &= ~(1LL << *result);
            ++result;
        }
        return result-*length;        
    }
    
    int* bitScan (u_int64_t x, int *length){
        *length = popCount(x);
        int *result = new int[*length];
        for(int i = 0; i < *length; ++i){
            *result = bitScanForward(x);
            x &= ~(1LL << *result);
            ++result;
        }
        return result-*length;  
    }
//    
//    int calcBitsSet(u_int64_t value){
//        int result = 0;
//        for(int pos = 0; pos < 64; pos++){
//            if(isSet(value, pos)){
//                ++result;
//            }
//        }
//        return result;
//    }
//
//    int* getBitsSet(u_int64_t value){
//        int *result = new int[calcBitsSet(value)];
//        int i = 0;
//        for(int pos = 0; pos < 64; pos++){
//            if(isSet(value, pos)){
//                result[i] = pos;
//                ++i;
//            }
//        }
//        return result;
//    }
//    
    
    //-----------------------------------------------------------------------pos

    int getPos(int x, int y){
        if(x >= 8){
            x = 7;
        }
        if(y >= 8){
            y = 7;
        }
        int pos = 0;
        switch (y) {
            case 0 : pos = 0;  x = (x >= 7 ? 6 : x); break;
            case 1 : pos = 7;  break;
            case 2 : pos = 15; x = (x >= 7 ? 6 : x); break;
            case 3 : pos = 22; break;
            case 4 : pos = 30; x = (x >= 7 ? 6 : x); break;
            case 5 : pos = 37; break;
            case 6 : pos = 45; x = (x >= 7 ? 6 : x); break;
            case 7 : pos = 52; break;
            default : return -1;
        }
        pos += x;
        return pos;
    }

    /**
     * dont forget delete[] pos
     * @param pos
     * @return [x,y]
     */
    int* getPos(int pos){
        if(pos < 7){
            int *result = new int[2]{pos, 0};
            return result;
        }else if(pos < 15){
            int *result = new int[2]{pos-7, 1};
            return result;
        }else if(pos < 22){
            int *result = new int[2]{pos-15, 2};
            return result;
        }else if(pos < 30){
            int *result = new int[2]{pos-22, 3};
            return result;
        }else if(pos < 37){
            int *result = new int[2]{pos-30, 4};
            return result;
        }else if(pos < 45){
            int *result = new int[2]{pos-37, 5};
            return result;
        }else if(pos < 52){
            int *result = new int[2]{pos-45, 6};
            return result;
        }else if(pos < 60){
            int *result = new int[2]{pos-52, 7};
            return result;
        }else{
            return 0;
        }
    }


    bool isInvalid(int pos){
        return pos >= 60;//FIT & (1LL << pos);
    }

//------------------------------------------------------------------------player

    int opponent(int id){
        return (id + 1) % 2;
    }
}