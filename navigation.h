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

int iterativeDeepening(Board board, int player, int depth, int firstguess, Move *resultMove);

#endif	/* NAVIGATION_H */

