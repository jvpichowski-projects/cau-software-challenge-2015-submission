/* 
 * File:   configuration.h
 * Author: jan
 *
 * Created on 19. Februar 2015, 16:46
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

//2^24 ungefähr 235mb : 16777216
//2 ^ 25 ungefähr 470mb : 33554432
//31250000
//62500000
#define ENTRY_MAP_SIZE 33554432

#define MAX_QUIESCENCE_SEARCH_DEEP 4
#define MAX_QUIESCENCE_SEARCH_DEEP_SETMOVES 6

#define TIME_FOR_CALC 1800

#define DYN_START_DEEP
#define START_DEEP_PERCENT 80
#define FIXED_START_DEEP
//for fixed start deep
#define START_DEEP 3

//#define mtdf
#define DYN_MTDF
#define DYN_MTDF_BORDER 40
//#define tt
//#define collision_stats
//#define null_window
//#define move_ordering

//#define move_order_stats

#ifdef move_order_stats
    #define move_cutoff_state
#endif

#endif	/* CONFIGURATION_H */

