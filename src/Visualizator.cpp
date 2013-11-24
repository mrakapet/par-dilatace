/* 
 * File:   Visualizator.cpp
 * Author: petr
 * 
 * Created on November 24, 2013, 4:20 PM
 */

#include "Visualizator.h"

using namespace std;

bool Visualizator::generateGraphVizualization(Node** nodes, int nodeCount) {
    ofstream graph;         
    File::openFile((char*)"graf.dot", graph); // vytvoreni souboru s definici grafu
    graph << "graph graf {\n";    
    // definice sousednosti uzlu:    1 -- 2 -- 3;
    for (int i=0; i < nodeCount; i++) {        
        for (int j=0; j < nodes[i]->getCountOfNeighbours(); j++) {
            if (i >= nodes[i]->getNeighbour(j)->getId()) {
                graph << "\t" << i << " -- " << nodes[i]->getNeighbour(j)->getId() << ";\n";
            }            
        }                        
    }          
    graph << "}\n";
    graph.close();    
    // vykresleni grafu
    int result = system("neato -Teps graf.dot -o graf.eps");
    if (result != 0) {
        throw "Generovani vizualizace grafu nedopadlo uspesne.";
    }
}

