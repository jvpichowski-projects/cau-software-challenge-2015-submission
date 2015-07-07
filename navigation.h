/* 
 * File:   navigation.h
 *
 * Created on 26. Februar 2015, 13:51
 */

#ifndef NAVIGATION_H
#define	NAVIGATION_H

#include "types.h"
#include "constants.h"
#include "config.h"
#include "boardstate.h"
#include "evaluation.h"

/**
 * Iterativ deepening framework. Calls search algorithm until time is up.
 * The serach deep is raised every iteration.
 * 
 * @param board the start board
 * @param player the player which should make the next turn
 * @param depth the maximal search depth
 * @param firstguess a first guess of the result. Needed for some iprovements of alphabeta
 * @param resultMove the resulting best move
 * @return the value of the resulting move
 */
int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove);

#endif	/* NAVIGATION_H */

