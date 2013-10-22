/* 
 * File:   wLengthMatrix.cpp
 * Author: petr
 * 
 * Created on October 22, 2013, 9:25 AM
 */

#include "WLengthMatrix.h"
#include <iostream>
#include <climits>

using namespace std;

WLengthMatrix::WLengthMatrix(int size) {
    this->diameter = 0;
    this->size = size;    
    matrix = new unsigned int*[size];
    for (int i=0; i < size; i++) {
        matrix[i] = new unsigned int[size];
        for (int j=0; j < size; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            }            
            else {
                matrix[i][j] = INT_MAX;
            }
        }
    }
}

WLengthMatrix::~WLengthMatrix() {
    for (int i=0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void WLengthMatrix::addNeighbour(int row, int column) {
    matrix[row][column] = 1;
}

void WLengthMatrix::aplyFloydWarshall() {
    int max = 0;
    for (int k = 0; k < size-1; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (matrix[i][j] > matrix[i][k] + matrix[k][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
                
            }
        }
    }
    int k = size - 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] > matrix[i][k] + matrix[k][j]) {
                matrix[i][j] = matrix[i][k] + matrix[k][j];
            }            
            if (matrix[i][j] > max) {
                max = matrix[i][j];
            }
        }
    }
    diameter = max;
}

int WLengthMatrix::getDiameter() {
    if (diameter == 0) {
        throw "Prumer grafu nebyl dosud zpocitan.";
    }
    return diameter;
}

/**
 * Tesna dolni mez: horni-cela-cast((n-1)/prumer(G)).
 * @return tesna dolni mez dilatace grafu
 */
int  WLengthMatrix::getLowerLimit() {
    if (diameter == 0) {
        throw "Prumer grafu nebyl dosud zpocitan.";
    }
    if (((size-1) % diameter) == 0) {   // napr: 10.0   -> muzeme rovnou vratit vysledek
        return (size-1) / diameter;
    }    
    return (size-1) / diameter + 1;     // napr: 10.1 -> musime zaokrouhlit nahoru       
}
