/* 
 * File:   Node.cpp
 * Author: petr
 * 
 * Created on October 20, 2013, 2:22 PM
 */

#include "Node.h"

using namespace std;

Node::Node(unsigned int nodeId){
    this->id = nodeId;
    this->neighbours = new vector<Node*>();
}    

Node::~Node(){
    delete this->neighbours;
}

unsigned int Node::getId(){
    return this->id;
}

Node* Node::getNeighbour(unsigned int index) {
    return this->neighbours->at(index);
}

void Node::addNeighbour(Node* node){
    this->neighbours->insert(this->neighbours->end(), node);
}

unsigned int Node::getCountOfNeighbours(){
    return (unsigned int)this->neighbours->size();
}


