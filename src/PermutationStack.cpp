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
        this->permutation[i] = 0;
        this->used[i] = false;
    }
    this->level=0;      // 0   
    this->endLevel=0;
    this->endVal=size;
    
}

PermutationStack::PermutationStack(const PermutationStack& orig) {
    length=orig.length;
    permutation=new int[length];
    memcpy(permutation, orig.permutation, length);
    used=new bool[length];    
    memcpy(used,orig.used,length);
    level=orig.level;
    endVal=orig.endVal;
    endLevel=orig.endLevel;
}

PermutationStack::~PermutationStack() {
    delete[] permutation;
    delete[] used;
}

bool PermutationStack::add(int i){
    if((!used[i]) /*&& ((level>endLevel) || (endVal>i)) && (!(level<endLevel))*/){
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

bool PermutationStack::isEnd() {
    if (level <= 0) {
        cout << "Level < 0  ========= level = " << level << endl;
        return true;
    }
    if (permutation[endLevel] == endVal) {
        cout << "Endvalue reached." << endl;
        return true;
    }
    return false;
    //return level<=endLevel; 
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

ostream& operator<<(ostream& os, PermutationStack* p) {
    os << "Permutace (level=" << p->getLevel() << "): ";
    for (int i=0; i < p->level; i++) {
        os << p->permutation[i] << " ";
    }
    return os;
}

int* PermutationStack::getPerm(){
    int* out = new int[length];
    memcpy(out,permutation,length * sizeof(int));    
    return out;
}

WrappedPermutation * PermutationStack::wrap(){
    int divLevel=-1,divValue=endVal;
    WrappedPermutation * out = NULL;
    for(int i=permutation[endLevel]+1;i<endVal;i++){
        if(!used[i]){
            divLevel=endLevel;
            endVal=i;
            break;
        }
    }
    for(int i = endLevel+1;(i<length-1) && (divLevel==-1);i++){
        for(int j = permutation[i]+1;j<length;j++){
            if(!used[j]){
                divLevel=i;
                endLevel=i;
                endVal=j;
                break;
            }
        }
    }
    if(divLevel!=-1){
        out = new WrappedPermutation(permutation,divLevel,divValue);
        out->start[divLevel]=endVal;
    }
    return out;
}

void PermutationStack::unwrap(WrappedPermutation * msg){
    this->endLevel=msg->endLevel;
    this->endVal=msg->endVal;
    this->level=endLevel+1;
    memcpy(this->permutation,msg->start,(endLevel+1)*sizeof(int));
}

bool PermutationStack::isFull(){
    return level==length;
}

void PermutationStack::setBound(int endLevel, int endValue) {
    this->endLevel = endLevel;
    this->endVal = endValue;
}

void PermutationStack::removeBound() {
    this->endLevel = 0;
    this->endVal = -1;
}