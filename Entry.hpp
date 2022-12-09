/*
Entry.hpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold variable data
*/
//#include "SymbolTable.hpp"

#ifndef ENTRY_H
#define ENTRY_H

class SymbolTable;

#include<string>
using std::string;


class Entry {
  public:
    Entry();
    Entry(string i, SymbolTable* t, int dim=0);
    string id;
    SymbolTable* type;
    int dims;
    bool equals(Entry* e);
    void print(int n);
    string getName();
    bool param;
  private:
    void printDims();
    void in(int n);
};

#endif
