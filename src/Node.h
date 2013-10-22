/* 
 * File:   Node.h
 * Author: petr
 *
 * Created on October 20, 2013, 2:22 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <vector>

using namespace std;

class Node {
public:
                 Node(unsigned int nodeId);   
                 ~Node();
    unsigned int getId();    
    void         addNeighbour(Node* node);
    Node*        getNeighbour(unsigned int index);
    unsigned int getCountOfNeighbours();
    
protected:
    unsigned int   id;
    vector<Node*>* neighbours;    
};

#endif	/* NODE_H */

