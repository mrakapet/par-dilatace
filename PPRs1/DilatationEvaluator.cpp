/* 
 * File:   DilatationEvaluator.cpp
 * Author: kuba
 * 
 * Created on October 15, 2013, 12:06 PM
 */

#include "DilatationEvaluator.h"
#include "Permutation.h"

DilatationEvaluator::DilatationEvaluator(Permutation& perm, Node* nodes) {
    this->nodes=nodes;
    this->permutation=perm;
}

DilatationEvaluator::DilatationEvaluator(const DilatationEvaluator& orig) {
    this->nodes=orig.nodes;
    this->permutation=orig.permutation;
}

DilatationEvaluator::~DilatationEvaluator() {
}

int DilatationEvaluator::evaluate(){
    int maxDilatation=0;
    int actual=permutation.getLevel();
    int * neighbors=nodes[actual].getNeighbors();
    for (int i = 0; i < nodes[actual].getLength(); i++) {
        int neighbor = neighbors[i];
        int neighborPos = permutation.getPosX(neighbor);
        if(neighbor[i]>actual)continue;
        if(maxDilatation < neighborPos){
            maxDilatation=neighborPos;
            if(maxDilatation>=currentMinMaxDilatation)
                return currentMinMaxDilatation;
        }
    }
    currentMinMaxDilatation=maxDilatation;
    return maxDilatation;
}

