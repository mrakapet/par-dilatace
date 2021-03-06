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

#include "File.h"
#include "Node.h"
#include "WLengthMatrix.h"
#include "DilatationEvaluator.h"
#include "PermutationStack.h"
#include "Visualizator.h"
#include "globals.h"

#ifdef LOCAL
        #include "CommunicationSerial.h"
#else
        #include "Communication.h"
#endif

#define CHECK 100
using namespace std;

void getParameters(int argc, char** argv) {
    if (argc != 2) {
        throw "Zadejte nazev souboru reprezentujiciho graf.";
    }
    if (!File::fileExist(argv[1])) {
        throw "Soubor neexistuje.";
    }
    File::openFile(argv[1], inputFile);
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
    if (processId == 0) {
        cout << processId << ": Uzly:" << endl;    
        for (int i=0; i < nodeCount; i++) {
            cout << nodes[i]->getId() << " -> ";        
            for (int j = 0; j < nodes[i]->getCountOfNeighbours(); j++) {
                cout << nodes[i]->getNeighbour(j)->getId() << ", ";
            }                
            cout << endl;
        }
        cout << endl;
    }
    else {
        cout << processId << ": Uzly: Nacteny." << endl;    
    }
}

/**
 * Uvolneni veskere alokovane pameti.
 */
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
    delete [] nodes;
    if (permutation != NULL) {
        delete permutation;
    }
    if (evaluator != NULL) {
        delete evaluator;
    }
    if (minPermutation != NULL) {
        delete [] minPermutation;
    }
    finalize();
}

/**
 * Nacteni grafu ze souboru a priprava datovych struktur.
 */
void loadData() {
    // zjisteni poctu uzlu grafu - prvni radek vstupniho souboru
    nodeCount = readCountOfNodes(inputFile);       
    cout << processId << ": Pocet uzlu grafu: " << nodeCount << endl;

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
    cout << processId << ": Prumer grafu: " << wMatrix->getDiameter() << endl;
    cout << processId << ": Dolni mez dilatace: " << lowerLimit << endl;
    delete wMatrix;       
    wMatrix = NULL; // kvuli testovani jestli uz je odstraneni

    // vizualizace grafu - na Windows asi nebude fungovat
    //Visualizator::generateGraphVizualization(nodes, nodeCount);
}


void printPermutation(int * perm,int size){ 
    cout<<processId<< ": ";
    for (int i = 0; i < size; i++) {        
        cout << perm[i] << " ";
    }
    cout<<endl;    
}

void generate() {
    int permDil, last;
    bool added = false;
    int checkMsg=0, round=0;
    //permutation->add(0);
    while(!finished && dilatation > lowerLimit /*&& !permutation->isEnd()*/) { // dokud neni zasobnik prazdny        
        permDil = evaluator->evaluate();        
        //cout <<processId<< ": "<< permutation;        
        //cout <<processId<< ": "<< (permDil >= dilatation ? "|" : "") << "\t-> " << permDil << endl;                
        if (permDil < dilatation && permutation->isFull()) {  // kompletní permutace s dilatací lepší než dosud nalezená          
            dilatation = permDil;            
            if (minPermutation != NULL) {
                delete [] minPermutation;                
            }
            minPermutation = permutation->getPerm();            
            cout <<processId<< ":---Aktualni minimalni dilatace: " << dilatation << "\t" << permutation << endl;         
            sendBest(); //posílám ostatním svůj nejlepší výsledek
            if (dilatation <= lowerLimit) {
                cout <<processId<< ":Nalezena permutace s dilataci rovne spodni mezi." << endl;
                sendTerminate(); //ukončujeme to
                break;
            }
        }    
        if (!permutation->isFull() && permDil < dilatation) {   // pokud mam volne pozice
            int i = 0;
            while (!permutation->add(i) && i < nodeCount) { // pridam jako dalsi znak 0-ty uzel
                i++;
            }            
        }
        else {  // pokud nemuzu pridavat -> musim splhat nahoru
           added = false;
           while (!added && !permutation->isEnd()) {               
               last = permutation->getTop() + 1;
               permutation->removeTop();                   
               while (!added && last < nodeCount) {
                   added = permutation->add(last);
                   last++;
               }
           }
        }
                        
        if(permutation->isEnd()){ //pokud mi došla práce, skusím si říci o další
            int *in = getWork();
            if(finished) {
                break;
            }
            permutation->unwrap(in);
        }
        if(checkMsg%CHECK==0) { // pokud prisly nejake nove zpravy
            checkForMsg(); // zpracuj zpravy
        }        
        checkMsg++;     // inkrementace pocitadla provedenych cyklu        
    }    
}

void testByParts() {
    permutation->setBound(0, 1);
    generate();
    cout << "---------------------------------------------------------" << endl;
    permutation->setBound(0,2);
    generate();
    cout << "---------------------------------------------------------" << endl;
    permutation->setBound(0,3);
    generate();
    cout << "---------------------------------------------------------" << endl;
    permutation->setBound(0,4);
    generate();
    cout << "---------------------------------------------------------" << endl;
    permutation->removeBound();
    generate();
}

/*
 * 
 */
int main(int argc, char** argv) {    
    double startTime, stopTime;
    try {
        initialize(argc, argv);        
        cout<<processId<<": started"<<endl; 
        barier();
        startTime = time();
        
        // nacteni dat
        getParameters(argc, argv);                       
        loadData();
        cout << endl;
        
        // vypocet dilatace
        permutation = new PermutationStack(nodeCount);
        evaluator = new DilatationEvaluator(permutation, nodes);
        //evaluator->setMinDilatation(INT_MAX);
        
        int start = nodeCount/processNumber * processId; //nastavení hranic části náležících danému procesu
        int end = nodeCount/processNumber * (processId+1);
        permutation->add(start);
        permutation->setBound(0,end);        
        
        //začínáme
        //testByParts();
        generate();
        
        cout <<processId<< ": Posledni vygenerovana permutace:\t" << permutation << endl;
                
    }
    catch (const char * e) {
        cout <<processId<< ": Chyba: " << e << endl;                
        cleanUp(); // uklid
        return 1;
    }   
 
    barier();
    if(processId==0){    
        cout <<processId<< ":\nDilatace grafu je " << dilatation << ":" << endl;
        printPermutation(minPermutation, nodeCount);
        stopTime = time();
        printf("%d: time: %f s.\n", processId, stopTime - startTime);
    }
    cleanUp();  // uklid
      
    cout<<processId<<": terminated"<<endl;
    return 0;
}

