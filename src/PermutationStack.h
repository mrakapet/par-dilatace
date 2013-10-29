/* 
 * File:   Permutator.h
 * Author: kuba
 *
 * Created on October 14, 2013, 10:28 PM
 */

#ifndef PERMUTATIONSTACK_H
#define	PERMUTATIONSTACK_H

#include <iostream>
#include "Permutation.h"

class PermutationStack: public Permutation{
public:
    PermutationStack(int size);
    PermutationStack(const PermutationStack& orig);
    ~PermutationStack();
    int getTop();
    bool add(int i);
    bool removeTop();
    int getPosX(int x);
    int getLevel();
    
    void print(bool eol = true);    
    
private:
    int * permutation;
    bool * used;
    int length;
    int level;
    

};

#endif	/* PERMUTATORSTACK_H */

