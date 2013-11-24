/* 
 * File:   File.h
 * Author: petr
 *
 * Created on November 24, 2013, 4:10 PM
 * Trida pro zjednoduseni prace se soubory.
 */

#ifndef FILE_H
#define	FILE_H

#include <fstream>

using namespace std;

class File {
public:
    bool static fileExist (const char * fileName);
    void static openFile(char* fileName, ifstream& file);
    void static openFile(char* fileName, ofstream& file);

};

#endif	/* FILE_H */

