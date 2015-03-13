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
            cout << ((field >> i) & 1ULL);
        }
         cout << endl;
    }

//--------------------------------------------------------------------------move

    u_int64_t initEmptyFields(){        
        return (1LL << 63) | (1LL << 62) | (1LL << 61) | (1LL << 60);
    }
    
    u_int64_t genMoveField(int pos, u_int64_t used){
        
        int right_shift = 59-pos;
        
        
//bool f;         // conditional flag
//unsigned int m; // the bit mask
//unsigned int w; // the word to modify:  if (f) w |= m; else w &= ~m; 
//w ^= (-f ^ w) & m;
//w = (w & ~m) | (-f & m);
        
//    u_int64_t var = 0ULL;
//    u_int64_t pattern = (1ULL << 15);
//    u_int64_t nonfree = 0;//pattern;//0;//pattern;
//    var |= (-(!(pattern & nonfree))) & pattern; //- sets all bits to 1 or to 0
//    Tools::printField(var);
        
        u_int64_t dr = 0ULL;
        //dr = (ROW_TL_DR << pos);// | */(ROW_TL_DR_LAST >> right_shift);
        //dr &= ~(1LL << pos);          //No moves to own position anymore.
        u_int64_t dr_1 = (ROW_DR_1 << pos);
        dr |= (-(!((dr_1 & LEFT_OUTER_BORDER) || (dr_1 & used)))) & dr_1;
        u_int64_t dr_2 = (ROW_DR_2 << pos);
        dr |= (-(!((dr_2 & LEFT_OUTER_BORDER) || (dr_2 & used)))) & dr_2;
        u_int64_t dr_3 = (ROW_DR_3 << pos);
        dr |= (-(!((dr_3 & LEFT_OUTER_BORDER) || (dr_3 & used)))) & dr_3;
        u_int64_t dr_4 = (ROW_DR_4 << pos);
        dr |= (-(!((dr_4 & LEFT_OUTER_BORDER) || (dr_4 & used)))) & dr_4;
        u_int64_t dr_5 = (ROW_DR_5 << pos);
        dr |= (-(!((dr_5 & LEFT_OUTER_BORDER) || (dr_5 & used)))) & dr_5;
        u_int64_t dr_6 = (ROW_DR_6 << pos);
        dr |= (-(!((dr_6 & LEFT_OUTER_BORDER) || (dr_6 & used)))) & dr_6;
        u_int64_t dr_7 = (ROW_DR_7 << pos);
        dr |= (-(!((dr_7 & LEFT_OUTER_BORDER) || (dr_7 & used)))) & dr_7;
        
        u_int64_t tl = 0ULL;
        u_int64_t tl_1 = (ROW_TL_1 >> right_shift);
        tl |= (-(!((tl_1 & RIGHT_OUTER_BORDER) || (tl_1 & used)))) & tl_1;
        u_int64_t tl_2 = (ROW_TL_2 >> right_shift);
        tl |= (-(!((tl_2 & RIGHT_OUTER_BORDER) || (tl_2 & used)))) & tl_2;
        u_int64_t tl_3 = (ROW_TL_3 >> right_shift);
        tl |= (-(!((tl_3 & RIGHT_OUTER_BORDER) || (tl_3 & used)))) & tl_3;
        u_int64_t tl_4 = (ROW_TL_4 >> right_shift);
        tl |= (-(!((tl_4 & RIGHT_OUTER_BORDER) || (tl_4 & used)))) & tl_4;
        u_int64_t tl_5 = (ROW_TL_5 >> right_shift);
        tl |= (-(!((tl_5 & RIGHT_OUTER_BORDER) || (tl_5 & used)))) & tl_5;
        u_int64_t tl_6 = (ROW_TL_6 >> right_shift);
        tl |= (-(!((tl_6 & RIGHT_OUTER_BORDER) || (tl_6 & used)))) & tl_6;
        u_int64_t tl_7 = (ROW_TL_7 >> right_shift);
        tl |= (-(!((tl_7 & RIGHT_OUTER_BORDER) || (tl_7 & used)))) & tl_7;
        
        u_int64_t dl = 0ULL;
        u_int64_t dl_1 = (ROW_DL_1 << pos);
        dl |= (-(!((dl_1 & RIGHT_OUTER_BORDER) || (dl_1 & used)))) & dl_1;
        u_int64_t dl_2 = (ROW_DL_2 << pos);
        dl |= (-(!((dl_2 & RIGHT_OUTER_BORDER) || (dl_2 & used)))) & dl_2;
        u_int64_t dl_3 = (ROW_DL_3 << pos);
        dl |= (-(!((dl_3 & RIGHT_OUTER_BORDER) || (dl_3 & used)))) & dl_3;
        u_int64_t dl_4 = (ROW_DL_4 << pos);
        dl |= (-(!((dl_4 & RIGHT_OUTER_BORDER) || (dl_4 & used)))) & dl_4;
        u_int64_t dl_5 = (ROW_DL_5 << pos);
        dl |= (-(!((dl_5 & RIGHT_OUTER_BORDER) || (dl_5 & used)))) & dl_5;
        u_int64_t dl_6 = (ROW_DL_6 << pos);
        dl |= (-(!((dl_6 & RIGHT_OUTER_BORDER) || (dl_6 & used)))) & dl_6;
        u_int64_t dl_7 = (ROW_DL_7 << pos);
        dl |= (-(!((dl_7 & RIGHT_OUTER_BORDER) || (dl_7 & used)))) & dl_7;
        
        
        u_int64_t tr = 0ULL;
        u_int64_t tr_1 = (ROW_TR_1 >> right_shift);
        tr |= (-(!((tr_1 & LEFT_OUTER_BORDER) || (tr_1 & used)))) & tr_1;
        u_int64_t tr_2 = (ROW_TR_2 >> right_shift);
        tr |= (-(!((tr_2 & LEFT_OUTER_BORDER) || (tr_2 & used)))) & tr_2;
        u_int64_t tr_3 = (ROW_TR_3 >> right_shift);
        tr |= (-(!((tr_3 & LEFT_OUTER_BORDER) || (tr_3 & used)))) & tr_3;
        u_int64_t tr_4 = (ROW_TR_4 >> right_shift);
        tr |= (-(!((tr_4 & LEFT_OUTER_BORDER) || (tr_4 & used)))) & tr_4;
        u_int64_t tr_5 = (ROW_TR_5 >> right_shift);
        tr |= (-(!((tr_5 & LEFT_OUTER_BORDER) || (tr_5 & used)))) & tr_5;
        u_int64_t tr_6 = (ROW_TR_6 >> right_shift);
        tr |= (-(!((tr_6 & LEFT_OUTER_BORDER) || (tr_6 & used)))) & tr_6;
        u_int64_t tr_7 = (ROW_TR_7 >> right_shift);
        tr |= (-(!((tr_7 & LEFT_OUTER_BORDER) || (tr_7 & used)))) & tr_7;

        
        u_int64_t r = 0ULL;
        u_int64_t r_1 = (ROW_R_1 << pos);
        r |= (-(!((r_1 & LEFT_BORDER) || (r_1 & used)))) & r_1;
        u_int64_t r_2 = (ROW_R_2 << pos);
        r |= (-(!((r_2 & LEFT_BORDER) || (r_2 & used)))) & r_2;
        u_int64_t r_3 = (ROW_R_3 << pos);
        r |= (-(!((r_3 & LEFT_BORDER) || (r_3 & used)))) & r_3;
        u_int64_t r_4 = (ROW_R_4 << pos);
        r |= (-(!((r_4 & LEFT_BORDER) || (r_4 & used)))) & r_4;
        u_int64_t r_5 = (ROW_R_5 << pos);
        r |= (-(!((r_5 & LEFT_BORDER) || (r_5 & used)))) & r_5;
        u_int64_t r_6 = (ROW_R_6 << pos);
        r |= (-(!((r_6 & LEFT_BORDER) || (r_6 & used)))) & r_6;
        u_int64_t r_7 = (ROW_R_7 << pos);
        r |= (-(!((r_7 & LEFT_BORDER) || (r_7 & used)))) & r_7;
        
        
        u_int64_t l = 0ULL;
        u_int64_t l_1 = (ROW_L_1 >> right_shift);
        l |= (-(!((l_1 & RIGHT_BORDER) || (l_1 & used)))) & l_1;
        u_int64_t l_2 = (ROW_L_2 >> right_shift);
        l |= (-(!((l_2 & RIGHT_BORDER) || (l_2 & used)))) & l_2;
        u_int64_t l_3 = (ROW_L_3 >> right_shift);
        l |= (-(!((l_3 & RIGHT_BORDER) || (l_3 & used)))) & l_3;
        u_int64_t l_4 = (ROW_L_4 >> right_shift);
        l |= (-(!((l_4 & RIGHT_BORDER) || (l_4 & used)))) & l_4;
        u_int64_t l_5 = (ROW_L_5 >> right_shift);
        l |= (-(!((l_5 & RIGHT_BORDER) || (l_5 & used)))) & l_5;
        u_int64_t l_6 = (ROW_L_6 >> right_shift);
        l |= (-(!((l_6 & RIGHT_BORDER) || (l_6 & used)))) & l_6;
        u_int64_t l_7 = (ROW_L_7 >> right_shift);
        l |= (-(!((l_7 & RIGHT_BORDER) || (l_7 & used)))) & l_7;
        
        u_int64_t result = dr | tl | dl | tr | r | l;
        result &= ~FIT;
        return result;
        
    }
    
//    u_int64_t getMoveField(int pos, u_int64_t used){
//        
//        //------------------------------top left to down right------------------
//        
//        int right_shift = 59-pos;
//        
//        u_int64_t tl_dr = (ROW_TL_DR << pos) | (ROW_TL_DR_LAST >> right_shift);
//        
//        int l = 0;
//        int *hits = Tools::bitScan(tl_dr & STOP_TOP_LEFT, &l);
//        for(int i = 0; i < l; ++i){
//            if(*hits <= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tl_dr = tl_dr >> *hits;
//                tl_dr = tl_dr << *hits;
//            }
//            ++hits;
//        }
//        hits-=l;
//        delete[] hits;
//        
//        hits = Tools::bitScan(tl_dr & STOP_DOWN_RIGHT, &l);
//        int i;
//        for(i = 0; i < l; ++i){
//            if(*hits >= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tl_dr = tl_dr << 63-*hits;
//                tl_dr = tl_dr >> 63-*hits;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        hits = Tools::bitScan(tl_dr & used, &l);
//        for(i = 0; i < l; ++i){
//            if(*hits < pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tl_dr = tl_dr >> *hits+1;
//                tl_dr = tl_dr << *hits+1;
//            }else if(*hits > pos){
//                tl_dr = tl_dr << 63-*hits+1;
//                tl_dr = tl_dr >> 63-*hits+1;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        //----------------------top right to down left--------------------------
//        
//        u_int64_t tr_dl = (ROW_TR_DL << pos) | (ROW_TR_DL_LAST >> right_shift);
//        hits = Tools::bitScan(tr_dl & STOP_TOP_RIGHT, &l);
//        for(int i = 0; i < l; ++i){
//            if(*hits <= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tr_dl = tr_dl >> *hits;
//                tr_dl = tr_dl << *hits;
//            }
//            ++hits;
//        }
//        hits-=l;
//        delete[] hits;
//        
//        hits = Tools::bitScan(tr_dl & STOP_DOWN_LEFT, &l);
//        for(i = 0; i < l; ++i){
//            if(*hits >= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tr_dl = tr_dl << 63-*hits;
//                tr_dl = tr_dl >> 63-*hits;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        hits = Tools::bitScan(tr_dl & used, &l);
//        for(i = 0; i < l; ++i){
//            if(*hits < pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                tr_dl = tr_dl >> *hits+1;
//                tr_dl = tr_dl << *hits+1;
//            }else if(*hits > pos){
//                tr_dl = tr_dl << 63-*hits+1;
//                tr_dl = tr_dl >> 63-*hits+1;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        //----------------------left to right-----------------------------------
//        
//        u_int64_t l_r = (ROW_LR << pos) | (ROW_LR_LAST >> right_shift);
//        hits = Tools::bitScan(l_r & STOP_LEFT, &l);
//        for(int i = 0; i < l; ++i){
//            if(*hits <= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                l_r = l_r >> *hits;
//                l_r = l_r << *hits;
//            }
//            ++hits;
//        }
//        hits-=l;
//        delete[] hits;
//        
//        hits = Tools::bitScan(l_r & STOP_RIGHT, &l);
//        for(i = 0; i < l; ++i){
//            if(*hits >= pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                l_r = l_r << 63-*hits;
//                l_r = l_r >> 63-*hits;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        hits = Tools::bitScan(l_r & used, &l);
//        for(i = 0; i < l; ++i){
//            if(*hits < pos){
//                //cout << "Pos " << *hits << " strtPos " << pos << endl;
//                l_r = l_r >> *hits+1;
//                l_r = l_r << *hits+1;
//            }else if(*hits > pos){
//                l_r = l_r << 63-*hits+1;
//                l_r = l_r >> 63-*hits+1;
//                //always from low to heigh, all next hits are behind this
//                break;
//            }
//            ++hits;
//        }
//        hits-=i;
//        delete[] hits;
//        
//        //------------------------combine lines---------------------------------
//        
//        u_int64_t result = tl_dr | tr_dl | l_r;
//        result &= ~FIT;
//        result &= ~used;
//        
//        result &= ~(1LL << pos);          //No moves to own position anymore.
//        
//        //printField(result);
//        
//        return result;
//    }
    
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
}