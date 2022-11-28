#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

#include "Entry.hpp"
#include "Type.hpp"

class SymbolTable {
  public:
    SymbolTable(SymbolTable* p);

    string id;
    Type* type;
    int value;
    SymbolTable* parent;
    SymbolTable* next;//for constructors and methods

    SymbolTable* lookup(string name);
    int insert(string name, string type, int dims=0);
    int insert(string name, Type* t);
    int insert(string name, SymbolTable* t);
    int insert(string name, Entry* e);
    //int addDim(string name);
    void print();

  protected:
    void print(int& n);
    bool exists(string name);
  private:
    unordered_map<string, Entry*> vars;
    unordered_map<string, SymbolTable*> scope;
};

void printDims(int n);
void in(int n);

#endif
