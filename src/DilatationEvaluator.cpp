/* 
 * File:   DilatationEvaluator.cpp
 * Author: kuba
 * 
 * Created on October 15, 2013, 12:06 PM
 */

#include "DilatationEvaluator.h"
#include "Permutation.h"
#include "Node.h"

DilatationEvaluator::DilatationEvaluator(Permutation& perm, Node* nodes) {
    this->nodes=nodes;
    this->permutation=perm;
}

DilatationEvaluator::~DilatationEvaluator() {
}

int DilatationEvaluator::evaluate(){
    int maxDistance=0;
    int actual=permutation.getLevel();
    for (int i = 0; i < nodes[actual].getCountOfNeighbours(); i++) {
        int neighbor = nodes[actual].getNeighbour(i)->getId();
        if(neighbor>actual)continue;
        int distance = permutation.getPosX(neighbor)-permutation.getTop();
        if(distance < 0)distance*=-1;
        if(maxDistance < distance){
            maxDistance=distance;
            if(maxDistance>=currentMinDilatation)
                return currentMinDilatation;
        }
    }
    currentMinDilatation=maxDistance;
    return maxDistance;
}

void DilatationEvaluator::setMinDilatation(int dil){
    currentMinDilatation=dil;
}