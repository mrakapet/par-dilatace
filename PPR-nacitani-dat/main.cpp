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

#include "Node.h"

using namespace std;

unsigned int    nodeCount = 0;
Node**          nodes = NULL;
ifstream        file;

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
    file.open(fileName, ios::binary | ios::out);
    if (!file.is_open()) {
        throw "Nepodarilo se otevrit/vytvorit vystupni soubor.";
    }
}

void nactiParametry(int argc, char** argv) {
    if (argc != 2) {
        throw "Zadejte nazev souboru reprezentujiciho graf.";
    }
    if (!fileExist(argv[1])) {
        throw "Soubor neexistuje.";
    }
    openFile(argv[1], file);
}

void deleteNodes() {
    for (int i=0; i<nodeCount; i++) {
        delete nodes[i];
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

/*
 * 
 */
int main(int argc, char** argv) {    
    try {
        nactiParametry(argc, argv);
    }
    catch (const char * e) {
        cout << "Chyba: " << e << endl;
        return 1;
    }
    
    char znak;
    file.get(znak);
    cout << "Obsah souboru:" << endl;
    int n = 0;
    // nacteni poctu uzlu grafu    
    while (file.good() && (znak != '\n')) {                
        n = n*10 + (short)znak - 48;     // odecteni nuly '0' = 48
        // cout << (short)znak << " " << n << endl;        
        file.get(znak);
    }
    cout << "Pocet uzlu grafu: " << n << endl;        
    
    nodeCount = n;       
    nodes = new Node*[nodeCount];
    for (int i=0; i < nodeCount; i++) {
        nodes[i] = new Node(i);
    }    
    
    try {
        for (int i=0; i < nodeCount; i++) {            
            for (int j=0; j < nodeCount; j++) {
                file.get(znak);
                //cout << (short)znak << endl;
                if (!file.good()) {
                    throw "Chyba cteni souboru";
                }                
                if (znak != '0' && znak != '1') {
                    throw "Neplatny obsah souboru. 1";
                }
                if (znak == '1') {      // pridani souseda
                    nodes[i]->addNeighbour(nodes[j]);
                }                                
            }
            // kontrola jestli jsem na konci radku
            file.get(znak);
            if (znak != '\n') {
                throw "Neplatny obsah souboru. 2";
            }
        }
    }
    catch (const char* e) {
        cout << "Chyba: " << e << endl;
        //TODO tady se musi uklidit pamet
        deleteNodes();
        return 1;
    }
    
    printNodes();
    
    
    // Pokud je nastaven failbit a není konec souboru, došlo k chybě souboru.
    if (file.fail() && !file.eof()) {
        cout << "Chyba: Chyba pri cteni souboru.";
        file.close();
        return 1;
    }
    
    file.close();    
    deleteNodes();
    cout << "OK" << endl;             
    return 0;
}

