/* 
 * File:   constants.h
 * Author: jan
 *
 * Created on 15. Februar 2015, 20:43
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <stdlib.h>

#define ID_WE 1
#define ID_OPPONENT 0

#define MIN_AB_VALUE -10000000
#define MAX_AB_VALUE  10000000

#define INVALID_POS 60
#define EMTY_POS 61

#define TOP_LEFT -8
#define TOP_RIGHT -7
#define LEFT -1
#define RIGHT 1
#define BOTTOM_LEFT 7
#define BOTTOM_RIGHT 8

#define EXACT_VALUE 0
#define LOWERBOUND 1
#define UPPERBOUND 2

const u_int64_t FIT = (1LL << 63) | (1LL << 62) | (1LL << 61) | (1LL << 60);

const u_int64_t TOP_LINE = (1LL << 0) | (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6);
const u_int64_t DOWN_LINE = (1LL << 52) | (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58) | (1LL << 59);
const u_int64_t LEFT_OUTER_BORDER = (1LL << 7) | (1LL << 22) | (1LL << 37) | (1LL << 52);// always (x-7)%15 = 0;
const u_int64_t RIGHT_OUTER_BORDER = (1LL << 14) | (1LL << 29) | (1LL << 44) | (1LL << 59);// always (x-14)%15 = 0;
const u_int64_t LEFT_INNER_BORDER = (1LL << 0) | (1LL << 15) | (1LL << 30) | (1LL << 45); // always x%15 = 0;
const u_int64_t RIGHT_INNER_BORDER = (1LL << 6) | (1LL << 21) | (1LL << 36) | (1LL << 51);//always (x-6)%15 = 0;

const u_int64_t STOP_TOP_LEFT = LEFT_OUTER_BORDER | TOP_LINE | FIT;
const u_int64_t STOP_TOP_RIGHT = RIGHT_OUTER_BORDER | TOP_LINE | FIT;  
const u_int64_t STOP_DOWN_LEFT = LEFT_OUTER_BORDER | DOWN_LINE | FIT;
//#define STOP_DOWN_LEFT 18442240611525329024
const u_int64_t STOP_DOWN_RIGHT = RIGHT_OUTER_BORDER | DOWN_LINE | FIT;
const u_int64_t STOP_LEFT = LEFT_INNER_BORDER | LEFT_OUTER_BORDER | FIT;
const u_int64_t STOP_RIGHT = RIGHT_INNER_BORDER | RIGHT_OUTER_BORDER | FIT;

//#define FIT 8444249301319680L

//starting at 0
const u_int64_t ROW_TL_DR =  1LL | (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32) | (1LL << 40) | (1LL << 48) | (1LL << 56);
//ending at 59
const u_int64_t ROW_TL_DR_LAST = (1LL << 3) | (1LL << 11) | (1LL << 19) | (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51) | (1LL << 59);
//u_int64_t move = ROW_TL_DR << pos | ROW_TL_DR >> 59-pos
const u_int64_t ROW_TR_DL = 1LL | (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28) | (1LL << 35) | (1LL << 42) | (1LL << 49);
const u_int64_t ROW_TR_DL_LAST = (1LL << 59) | (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31) | (1LL << 24) | (1LL << 17) | (1LL << 10);
const u_int64_t ROW_LR = 1LL | (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6) | (1LL << 7);//(1LL << 7) | (1LL << 8) | (1LL << 9) | (1LL << 10) | (1LL << 11) | (1LL << 12) | (1LL << 13) | (1LL << 14);
const u_int64_t ROW_LR_LAST = (1LL << 52) | (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58) | (1LL << 59);


#endif	/* CONSTANTS_H */

