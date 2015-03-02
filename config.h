/* 
 * File:   configuration.h
 * Author: jan
 *
 * Created on 19. Februar 2015, 16:46
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

//#define DEBUG_MOVE_ORDERING

//#define mtdf
#define dyn_mtdf
#define tt
#define secure_tt
//#define collision_stats
//#define null_window
//#define move_ordering

//#define move_order_stats

#ifdef move_order_stats
    #define move_cutoff_state
#endif


//2^24 ungefähr 235mb : 16777216
//2 ^ 25 ungefähr 470mb : 33554432
//31250000
//62500000
//
#ifdef secure_tt
#define ENTRY_MAP_SIZE 16777216
#else
#define ENTRY_MAP_SIZE 33554432
#endif
#define TIME_FOR_CALC 1900

#define DYN_MTDF_BORDER 40

#define DYN_START_DEEP
#define START_DEEP_PERCENT 80
#define FIXED_START_DEEP
//for fixed start deep
#define START_DEEP 3

#endif	/* CONFIGURATION_H */

