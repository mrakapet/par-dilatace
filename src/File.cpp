/* 
 * File:   File.cpp
 * Author: petr
 * 
 * Created on November 24, 2013, 4:10 PM
 */

#include "File.h"

bool File::fileExist (const char * fileName) {
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
void File::openFile(char* fileName, ifstream& file) {
    file.open(fileName, ios::binary | ios::in);
    if (!file)
        throw "Vstupni soubor nenalezen!";
    if (file.peek() == EOF) {
        file.close();
        throw "Soubor je prazdny!";
    }
}
void File::openFile(char* fileName, ofstream& file) {
    file.open(fileName, ios::out);
    if (!file.is_open()) {
        throw "Nepodarilo se otevrit/vytvorit vystupni soubor.";
    }
}

