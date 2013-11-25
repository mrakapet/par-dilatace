/* 
 * File:   globals.h
 * Author: kuba
 *
 * Created on 25. listopad 2013, 16:32
 */

#ifndef GLOBALS_H
#define	GLOBALS_H
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>
#include "Node.h"
#include "WLengthMatrix.h"
#include "DilatationEvaluator.h"
#include "PermutationStack.h"
  
unsigned int            nodeCount = 0;
Node**                  nodes = NULL;
ifstream                inputFile;
WLengthMatrix*          wMatrix = NULL;
int                     lowerLimit = 0;
PermutationStack*       permutation = NULL;
DilatationEvaluator*    evaluator = NULL;
int                     dilatation = INT_MAX;
int*                    minPermutation = NULL;
bool                    finished = false;

#endif	/* GLOBALS_H */

