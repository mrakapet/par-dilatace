/* 
 * File:   WrapedPermutation.cpp
 * Author: kuba
 * 
 * Created on 19. listopad 2013, 10:40
 */

#include <string.h>

#include "WrapedPermutation.h"

WrappedPermutation::WrappedPermutation(int* start, int level, int end){
    this->start = new int[level];
    this->endLevel = level;
    this->endVal = end;
    memcpy(this->start, start, sizeof(int)*level);
}

WrappedPermutation::WrappedPermutation(const WrappedPermutation& orig) {
    this->endLevel=orig.endLevel;
    this->endVal=orig.endVal;
    this->start = new int[endLevel];
    memcpy(this->start,orig.start,endLevel*sizeof(int));
}

WrappedPermutation::~WrappedPermutation() {
    delete[] start;
}

