/* 
 * File:   main.cpp
 * Author: petr
 *
 * Created on October 20, 2013, 1:31 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

#include "Node.h"
#include "WLengthMatrix.h"
#include "DilatationEvaluator.h"
#include "PermutationStack.h"

using namespace std;
  
unsigned int            nodeCount = 0;
Node**                  nodes = NULL;
ifstream                inputFile;
WLengthMatrix*          wMatrix = NULL;
int                     lowerLimit = 0;
PermutationStack*       permutation = NULL;
DilatationEvaluator*    evaluator = NULL;
int                     dilatation = INT_MAX;

bool fileExist (const char * fileName) {
    // Existence souboru se overi pokusem o otevreni souboru.
    ifstream file (fileName, ios::binary | ios::in);
    if (file) { // existujici otevreny soubor se musi kvuli dalsimu pouziti zavrit
        file.close();
        return true;
    }
    else {
        return false;
    }
}
void openFile(char* fileName, ifstream& file) {
    file.open(fileName, ios::binary | ios::in);
    if (!file)
        throw "Vstupni soubor nenalezen!";
    if (file.peek() == EOF) {
        file.close();
        throw "Soubor je prazdny!";
    }
}
void openFile(char* fileName, ofstream& file) {
    file.open(fileName, ios::out);
    if (!file.is_open()) {
        throw "Nepodarilo se otevrit/vytvorit vystupni soubor.";
    }
}

void getParameters(int argc, char** argv) {
    if (argc != 2) {
        throw "Zadejte nazev souboru reprezentujiciho graf.";
    }
    if (!fileExist(argv[1])) {
        throw "Soubor neexistuje.";
    }
    openFile(argv[1], inputFile);
}

int readCountOfNodes(ifstream& file) {
    char znak;
    int n = 0;
    file.get(znak);                
    // nacteni poctu uzlu grafu    
    while (file.good() && (znak != '\n')) {                
        n = n*10 + (short)znak - 48;     // odecteni nuly '0' = 48
        //cout << (short)znak << " " << n << endl;        
        file.get(znak);
    }
    return n;    
}

void readNodesFromFile(ifstream& file, Node** nodes, WLengthMatrix* wMatrix, unsigned int nodeCount) {
    // nacteni a zpracovani matice sousednosti ze souboru
    char znak;
    for (int i=0; i < nodeCount; i++) {            
        for (int j=0; j < nodeCount; j++) { // nacteni jednoho radku
            file.get(znak);                
            if (file.fail()) {
                throw "Chyba cteni vstupniho souboru.";
            }                
            if (znak != '0' && znak != '1') {
                throw "Neplatny obsah vstupniho souboru.";
            }
            if (znak == '1') {      // pridani souseda
                nodes[i]->addNeighbour(nodes[j]);   // pridani souseda primo uzlu
                wMatrix->addNeighbour(i, j);        // vyznaceni souseda v matici w-delek
            }                                
        }            
        file.get(znak);
        if (znak != '\n') { // kontrola jestli jsem na konci radku
            throw "Neplatny obsah vstupniho souboru.";
        }
    }                

    // Pokud je nastaven failbit a není konec souboru, došlo k chybě souboru.
    if (inputFile.fail() && !inputFile.eof()) {
        throw "Chyba cteni vstupniho souboru";                        
    }
}

void printNodes() {
    cout << "\nUzly:" << endl;    
    for (int i=0; i < nodeCount; i++) {
        cout << nodes[i]->getId() << " -> ";        
        for (int j = 0; j < nodes[i]->getCountOfNeighbours(); j++) {
            cout << nodes[i]->getNeighbour(j)->getId() << ", ";
        }                
        cout << endl;
    }
    cout << endl;
}

/**
 * Vygenerovani graficke podoby grafu pomoci nastroje neato (graphviz). Nejprve
 * je vygenerovan soubor "graf.dot", ktery je vstupem programu neato, ktery
 * vygeneruje obrazek (.eps) grafu).
 * Na Windows asi nebude fungovat !!!
 * @param nodes pole vsech uzlu
 * @param nodeCount pocet  uzlu grafu
 */
bool generateGraphVizualization(Node** nodes, int nodeCount) {
    ofstream graph;         
    openFile((char*)"graf.dot", graph); // vytvoreni souboru s definici grafu
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

void cleanUp() {
    if (inputFile.is_open()) {
        inputFile.close();    
    }
    if (wMatrix != NULL) {
        delete wMatrix;    
    }
    if (nodes != NULL) {
        for (int i=0; i<nodeCount; i++) {
            delete nodes[i];
        }
    }    
    if (permutation != NULL) {
        delete permutation;
    }
    if (evaluator != NULL) {
        delete evaluator;
    }
}

/**
 * Nacteni grafu ze souboru a priprava datovych struktur.
 */
void loadData() {
    // zjisteni poctu uzlu grafu - prvni radek vstupniho souboru
    nodeCount = readCountOfNodes(inputFile);       
    cout << "Pocet uzlu grafu: " << nodeCount << endl;

    // alokace a priprava pole uzlu a matice w-delek
    wMatrix = new WLengthMatrix(nodeCount);
    nodes = new Node*[nodeCount];
    for (int i=0; i < nodeCount; i++) {
        nodes[i] = new Node(i);
    }    

    // nacteni uzlu z matice sousednosti ve vstupnim souboru
    readNodesFromFile(inputFile, nodes, wMatrix, nodeCount);        
    printNodes();

    // vypocet prumeru grafu a dolni meze dilatace
    wMatrix->aplyFloydWarshall();
    lowerLimit = wMatrix->getLowerLimit();
    cout << "Prumer grafu: " << wMatrix->getDiameter() << endl;
    cout << "Dolni mez dilatace: " << lowerLimit << endl;
    delete wMatrix;       
    wMatrix = NULL; // kvuli testovani jestli uz je odstraneni

    // vizualizace grafu - na Windows asi nebude fungovat
    //generateGraphVizualization(nodes, nodeCount);
}

void generate() {
    //permutation->print(false);
    if (permutation->getLevel() > 0) {
        int permDil = evaluator->evaluate();
        //if (permutation->getLevel()==nodeCount-1) { cout << "   -> " << permDil; }
        if (permDil > dilatation) {
            //cout << " - blocked";
            //cout << endl;
            permutation->removeTop();
            return;
        }
        if (permDil < dilatation && permDil >= lowerLimit && permutation->getLevel() == nodeCount-1) {
            dilatation = permDil;
        }                
    }        
    //cout << endl;    
    for (int i=0; i<nodeCount; i++) {
        if (permutation->add(i)) {
            generate();
        }
    }
    permutation->removeTop();
}

/*
 * 
 */
int main(int argc, char** argv) {    
    
    try {
        // nacteni dat
        getParameters(argc, argv);                       
        loadData();
        cout << endl;
        
        // vypocet dilatace
        permutation = new PermutationStack(nodeCount);
        evaluator = new DilatationEvaluator(permutation, nodes);
        //evaluator->setMinDilatation(INT_MAX);
//        for (int i=0; i < nodeCount; i++) {
//            for (int j=0; j < nodeCount; j++) {
//                permutation->add(j);  
//                permutation->print();                          
//                cout << "\t-> " << evaluator->evaluate() << endl;
//                permutation->removeTop();
//            }            
//        }
        generate();
                
    }
    catch (const char * e) {
        cout << "Chyba: " << e << endl;                
        cleanUp(); // uklid
        return 1;
    }   
    
    
    cleanUp();  // uklid
    cout << "\nDilatace grafu je " << dilatation << "." << endl;
    return 0;
}

