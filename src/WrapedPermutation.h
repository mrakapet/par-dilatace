/* 
 * File:   WrapedPermutation.h
 * Author: kuba
 *
 * Created on 19. listopad 2013, 10:40
 */

#ifndef WRAPEDPERMUTATION_H
#define	WRAPEDPERMUTATION_H

class WrappedPermutation {
public:
    WrappedPermutation();
    WrappedPermutation(int*start,int level,int end);
    WrappedPermutation(const WrappedPermutation& orig);
    ~WrappedPermutation();
    int endLevel;
    int endVal; 
    int* start;
private:
    
};

#endif	/* WRAPEDPERMUTATION_H */

