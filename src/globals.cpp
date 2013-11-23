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

using namespace std;
  
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
