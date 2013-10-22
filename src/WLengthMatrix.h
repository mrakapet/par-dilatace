/* 
 * File:   wLengthMatrix.h
 * Author: petr
 *
 * Created on October 22, 2013, 9:25 AM
 */

#ifndef WLENGTHMATRIX_H
#define	WLENGTHMATRIX_H

class WLengthMatrix {
public:
         WLengthMatrix(int size);    
        ~WLengthMatrix();    
    void addNeighbour(int row, int column);
    void aplyFloydWarshall();        
    int  getDiameter();
    int  getLowerLimit();
protected:
    unsigned int size;
    unsigned int** matrix;    
    unsigned int diameter;                
};

#endif	/* WLENGTHMATRIX_H */

