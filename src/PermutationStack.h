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

using namespace std;

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
    bool isEmpty();
    
    void print(bool eol = true);  
    friend ostream& operator<<(ostream& os, PermutationStack* p);
    
private:
    int * permutation;
    bool * used;
    int length;
    int level;
    

};

#endif	/* PERMUTATORSTACK_H */

