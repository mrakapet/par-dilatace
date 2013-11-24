/* 
 * File:   Visualizator.h
 * Author: petr
 *
 * Created on November 24, 2013, 4:20 PM
 * Trida pro generovani obsahu grafu pomoci programu graphviz::neato
 */

#ifndef VISUALIZATOR_H
#define	VISUALIZATOR_H

#include <cstdlib>
#include "File.h"
#include "Node.h"

using namespace std;

class Visualizator {
    
public:
    /**
    * Vygenerovani graficke podoby grafu pomoci nastroje neato (graphviz). Nejprve
    * je vygenerovan soubor "graf.dot", ktery je vstupem programu neato, ktery
    * vygeneruje obrazek (.eps) grafu).
    * Na Windows asi nebude fungovat !!!
    * @param nodes pole vsech uzlu
    * @param nodeCount pocet  uzlu grafu
    */
    bool static generateGraphVizualization(Node** nodes, int nodeCount);
    
};

#endif	/* VISUALIZATOR_H */

