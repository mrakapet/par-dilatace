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
    DilatationEvaluator(const DilatationEvaluator& orig);
    virtual ~DilatationEvaluator();
    int evaluate();
private:
    Permutation &permutation;
    Node * nodes;
    int currentMinMaxDilatation;
};

#endif	/* DILATATIONEVALUATOR_H */

