/* 
 * File:   constants.h
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


/*
 * surrounding fields are marked
 * e.g.
 * 
 *  0 0 0 0 0 0 0
 *   0 0 1 1 0 0
 *  0 0 1 x 1 0 0
 *   0 0 1 1 0 0
 *  0 0 0 0 0 0 0
 */
const int64_t _fieldsAround[60] = {
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


const u_int64_t FIT = (1LL << 63) | (1LL << 62) | (1LL << 61) | (1LL << 60);

/*
 *  1 1 1 1 1 1 1
 * 0 0 0 0 0 0 0 0
 *  ...
 */
const u_int64_t TOP_LINE = (1LL << 0) | (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6);

/*
 * ...
 *  0 0 0 0 0 0 0
 * 1 1 1 1 1 1 1 1
 */
const u_int64_t DOWN_LINE = (1LL << 52) | (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58) | (1LL << 59);

/*
 *  0 0 0 ...
 * 1 0 0 ...
 *  0 0 0 ...
 * 1 0 0 ...
 *  0 0 0 ...
 * 1 0 0 ...
 * ...
 */
const u_int64_t LEFT_OUTER_BORDER = (1LL << 7) | (1LL << 22) | (1LL << 37) | (1LL << 52);// always (x-7)%15 = 0;
const u_int64_t RIGHT_OUTER_BORDER = (1LL << 14) | (1LL << 29) | (1LL << 44) | (1LL << 59);// always (x-14)%15 = 0;

/*
 *  1 0 0 ...
 * 0 0 0 ...
 *  1 0 0 ...
 * 0 0 0 ...
 * ...
 */
const u_int64_t LEFT_INNER_BORDER = (1LL << 0) | (1LL << 15) | (1LL << 30) | (1LL << 45); // always x%15 = 0;
const u_int64_t RIGHT_INNER_BORDER = (1LL << 6) | (1LL << 21) | (1LL << 36) | (1LL << 51);//always (x-6)%15 = 0;

/*
 *  1 0 0 ...
 * 1 0 0 ...
 *  1 0 0 ...
 * 1 0 0 ...
 */
const u_int64_t LEFT_BORDER = LEFT_INNER_BORDER | LEFT_OUTER_BORDER;
const u_int64_t RIGHT_BORDER = RIGHT_OUTER_BORDER | RIGHT_INNER_BORDER;

/*
 *  1 1 1 1 ...
 * 1 0 0 0 ...
 *  1 0 0 0 ...
 * 1 0 0 0 ...
 * ...
 *           1 1 1
 */
const u_int64_t STOP_TOP_LEFT = LEFT_OUTER_BORDER | TOP_LINE | FIT;
const u_int64_t STOP_TOP_RIGHT = RIGHT_OUTER_BORDER | TOP_LINE | FIT;  
const u_int64_t STOP_DOWN_LEFT = LEFT_OUTER_BORDER | DOWN_LINE | FIT;
const u_int64_t STOP_DOWN_RIGHT = RIGHT_OUTER_BORDER | DOWN_LINE | FIT;
const u_int64_t STOP_LEFT = LEFT_INNER_BORDER | LEFT_OUTER_BORDER | FIT;
const u_int64_t STOP_RIGHT = RIGHT_INNER_BORDER | RIGHT_OUTER_BORDER | FIT;


//starting at 0
const u_int64_t ROW_TL_DR =  1LL | (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32) | (1LL << 40) | (1LL << 48) | (1LL << 56);
//ending at 59
const u_int64_t ROW_TL_DR_LAST = (1LL << 3) | (1LL << 11) | (1LL << 19) | (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51) | (1LL << 59);
//u_int64_t move = ROW_TL_DR << pos | ROW_TL_DR >> 59-pos
const u_int64_t ROW_TR_DL = 1LL | (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28) | (1LL << 35) | (1LL << 42) | (1LL << 49);
const u_int64_t ROW_TR_DL_LAST = (1LL << 59) | (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31) | (1LL << 24) | (1LL << 17) | (1LL << 10);
const u_int64_t ROW_LR = 1LL | (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6) | (1LL << 7);//(1LL << 7) | (1LL << 8) | (1LL << 9) | (1LL << 10) | (1LL << 11) | (1LL << 12) | (1LL << 13) | (1LL << 14);
const u_int64_t ROW_LR_LAST = (1LL << 52) | (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58) | (1LL << 59);

/*
 *  0 0 0 0 0 0 0
 * 0 1 0 0 ...
 *  0 1 0 ...
 * ...
 */
const u_int64_t ROW_DR_7 =  (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32) | (1LL << 40) | (1LL << 48) | (1LL << 56);
const u_int64_t ROW_DR_6 =  (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32) | (1LL << 40) | (1LL << 48);
const u_int64_t ROW_DR_5 =  (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32) | (1LL << 40);
const u_int64_t ROW_DR_4 =  (1LL << 8) | (1LL << 16) | (1LL << 24) | (1LL << 32);
const u_int64_t ROW_DR_3 =  (1LL << 8) | (1LL << 16) | (1LL << 24);
const u_int64_t ROW_DR_2 =  (1LL << 8) | (1LL << 16);
const u_int64_t ROW_DR_1 =  (1LL << 8);

/*
 * Row starting in the most top left place and ending at the bottom right corner
 *  0 0 0 1 0 0 0
 * 0 0 0 0 1 0 0 0
 *  0 0 0 0 1 0 0
 * ...
 * 0 0 0 0 0 0 0 1
 */
const u_int64_t ROW_TL_7 =  (1LL << 3) | (1LL << 11) | (1LL << 19) | (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51);
/*
 *  0 0 0 0 0 0 0
 * 0 0 0 0 1 0 0 0
 *  0 0 0 0 1 0 0
 * ...
 * 0 0 0 0 0 0 0 1
 */
const u_int64_t ROW_TL_6 =  (1LL << 11) | (1LL << 19) | (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51);
const u_int64_t ROW_TL_5 =  (1LL << 19) | (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51);
const u_int64_t ROW_TL_4 =  (1LL << 27) | (1LL << 35) | (1LL << 43) | (1LL << 51);
const u_int64_t ROW_TL_3 =  (1LL << 35) | (1LL << 43) | (1LL << 51);
const u_int64_t ROW_TL_2 =  (1LL << 43) | (1LL << 51);
const u_int64_t ROW_TL_1 =  (1LL << 51);

const u_int64_t ROW_DL_7 =  (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28) | (1LL << 35) | (1LL << 42) | (1LL << 49);
const u_int64_t ROW_DL_6 =  (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28) | (1LL << 35) | (1LL << 42);
const u_int64_t ROW_DL_5 =  (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28) | (1LL << 35);
const u_int64_t ROW_DL_4 =  (1LL << 7) | (1LL << 14) | (1LL << 21) | (1LL << 28);
const u_int64_t ROW_DL_3 =  (1LL << 7) | (1LL << 14) | (1LL << 21);
const u_int64_t ROW_DL_2 =  (1LL << 7) | (1LL << 14);
const u_int64_t ROW_DL_1 =  (1LL << 7);

const u_int64_t ROW_TR_7 = (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31) | (1LL << 24) | (1LL << 17) | (1LL << 10);
const u_int64_t ROW_TR_6 = (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31) | (1LL << 24) | (1LL << 17);
const u_int64_t ROW_TR_5 = (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31) | (1LL << 24);
const u_int64_t ROW_TR_4 = (1LL << 52) | (1LL << 45) | (1LL << 38) | (1LL << 31);
const u_int64_t ROW_TR_3 = (1LL << 52) | (1LL << 45) | (1LL << 38);
const u_int64_t ROW_TR_2 = (1LL << 52) | (1LL << 45);
const u_int64_t ROW_TR_1 = (1LL << 52);

/*
 *  0 1 1 1 1 1 1
 * 1 0 0 0 0 0 0 0
 * ...
 */
const u_int64_t ROW_R_7  = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6) | (1LL << 7);

/*
 *  0 1 1 1 1 1 1
 * 0 0 ...
 * ...
 */
const u_int64_t ROW_R_6  = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5) | (1LL << 6);
const u_int64_t ROW_R_5  = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4) | (1LL << 5);
const u_int64_t ROW_R_4  = (1LL << 1) | (1LL << 2) | (1LL << 3) | (1LL << 4);
const u_int64_t ROW_R_3  = (1LL << 1) | (1LL << 2) | (1LL << 3);
const u_int64_t ROW_R_2  = (1LL << 1) | (1LL << 2);
const u_int64_t ROW_R_1  = (1LL << 1);

/*
 * ...
 *  0 0 0 0 0 0 0
 * 1 1 1 1 1 1 1 0
 */
const u_int64_t ROW_L_7  = (1LL << 52) | (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_6  = (1LL << 53) | (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_5  = (1LL << 54) | (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_4  = (1LL << 55) | (1LL << 56) | (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_3  = (1LL << 56) | (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_2  = (1LL << 57) | (1LL << 58);
const u_int64_t ROW_L_1  = (1LL << 58);


#endif	/* CONSTANTS_H */

