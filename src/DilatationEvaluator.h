/* 
 * File:   DilatationEvaluator.h
 * Author: kuba
 *
 * Created on October 15, 2013, 12:06 PM
 */

#ifndef DILATATIONEVALUATOR_H
#define	DILATATIONEVALUATOR_H

#include "Evaluator.h"


class DilatationEvaluator : public Evaluator {
public:
    DilatationEvaluator(Permutation &perm, Node* nodes);
    virtual ~DilatationEvaluator();
    int evaluate();
    void setMinDilatation(int dil);
private:
    Permutation &permutation;
    Node * nodes;
    int currentMinDilatation;
};



#endif	/* DILATATIONEVALUATOR_H */

