/* 
 * File:   DilatationEvaluator.cpp
 * Author: kuba
 * 
 * Created on October 15, 2013, 12:06 PM
 */

#include "DilatationEvaluator.h"
#include <iostream>

DilatationEvaluator::DilatationEvaluator(Permutation* perm, Node** nodes) {
    this->nodes=nodes;
    this->permutation=perm;
}

DilatationEvaluator::~DilatationEvaluator() {
}

int DilatationEvaluator::evaluate(){
    int maxDistance=0;
    int actual=permutation->getLevel(); // posledni pridany uzel do permutace
    for (int i = 0; i < nodes[actual]->getCountOfNeighbours(); i++) {   // prochazeni sousedu aktualniho uzlu
        int neighbor = nodes[actual]->getNeighbour(i)->getId();        
        if(neighbor>actual)continue;
        int distance = permutation->getPosX(neighbor) - permutation->getTop();        
        if(distance < 0)distance*=-1;         
        //cout << "[" << actual << " - " << neighbor << " = " << distance << "]";
        if(maxDistance < distance){
            maxDistance=distance;
        }
    }
    //currentMinDilatation=maxDistance;
    return maxDistance;
}