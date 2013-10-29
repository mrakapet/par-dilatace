/* 
 * File:   Permutator.cpp
 * Author: kuba
 * 
 * Created on October 14, 2013, 10:28 PM
 */

#include <stdlib.h>
#include <string.h>
#include "PermutationStack.h"
using namespace std;

PermutationStack::PermutationStack(int size) {
    this->length=size;
    this->permutation=new int[size];
    this->used=new bool[size];
    for (int i=0; i < size; i++) {
        this->used[i] = false;
    }
    this->level=0;      // 0   
}

PermutationStack::PermutationStack(const PermutationStack& orig) {
    length=orig.length;
    permutation=new int[length];
    memcpy(permutation, orig.permutation, length);
    used=new bool[length];    
    memcpy(used,orig.used,length);
    level=orig.level;
}

PermutationStack::~PermutationStack() {
    delete[] permutation;
    delete[] used;
}

bool PermutationStack::add(int i){
    if(!used[i]){
        permutation[level++]=i;
        used[i]=true;
        return true;
    }
    return false;
}

int PermutationStack::getPosX(int x){
    if(x>=level)return -1;
    return permutation[x];    // -1
}

int PermutationStack::getTop(){
    return permutation[level-1];
}

bool PermutationStack::removeTop(){
    if(level==0)return false;
    level--;
    used[permutation[level]]=false;
    return true;
}

int PermutationStack::getLevel(){
    return level-1;     // -1
}

void PermutationStack::print(bool eol) {
    cout << "Permutace (level=" << getLevel() << "): ";
    for (int i=0; i < level; i++) {
        cout << permutation[i] << " ";
    }
    if (eol) {
        cout << endl;
    }    
}

const int* PermutationStack::getPerm(){
    int out[] = new int[length];
    memcpy(out,permutation,length);
    return out;
}
