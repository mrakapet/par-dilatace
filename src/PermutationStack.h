/* 
 * File:   Permutator.h
 * Author: kuba
 *
 * Created on October 14, 2013, 10:28 PM
 */

#ifndef PERMUTATOR_H
#define	PERMUTATOR_H
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
    
private:
    int * permutation;
    bool * used;
    int length;
    int level;
    

};

#endif	/* PERMUTATOR_H */

