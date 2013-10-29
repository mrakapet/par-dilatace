/* 
 * File:   PermutationStack.h
 * Author: kuba
 *
 * Created on October 15, 2013, 11:41 AM
 */

#ifndef PERMUTATOR_H
#define	PERMUTATOR_H

class Permutation {
public:
    virtual int getTop()=0; //return topmost item
    virtual bool add(int i)=0; //add item to top
    virtual bool removeTop()=0; //remove item from top
    virtual int getPosX(int x)=0; //return item on position x(from bottom)
    virtual int getLevel()=0; //return index of top item;    
};

#endif	/* PERMUTATION_H */

