/* 
 * File:   configuration.h
 * Author: jan
 *
 * Created on 19. Februar 2015, 16:46
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

//#define DEBUG_MOVE_ORDERING

//#define quiscens_search
#define ordered_setmoves
#define ordered_runmoves
//#define collision_stats
//#define null_window

#define bigfield_eval

#define limit_move_gen
#define MOVE_GEN_LIMIT 10

#define move_order_stats

#ifdef move_order_stats
    #define move_cutoff_state
#endif

#define TIME_FOR_CALC 1800

#define DYN_START_DEEP
#define START_DEEP_PERCENT 80
#define FIXED_START_DEEP
//for fixed start deep
#define START_DEEP 3

#endif	/* CONFIGURATION_H */

