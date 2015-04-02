/* 
 * File:   visual.h
 * Author: jan
 *
 * Created on 15. Februar 2015, 20:47
 */

#ifndef VISUAL_H
#define	VISUAL_H

#include <cstdio>
#include <iostream>

#include "config.h"
#include "globals.h"

void theBreadfish();
void theShark();
void theVictory();

#ifdef move_order_stats
void printMoveOrderStats();
void printSortedMoveList(Move* move, int length);
#endif

#endif	/* VISUAL_H */

