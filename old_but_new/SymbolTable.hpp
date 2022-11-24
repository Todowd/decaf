#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

#include "Symbol.hpp"

class SymbolTable {
  public:
    SymbolTable();
    SymbolTable(Symbol* i, int t, SymbolTable* p=nullptr);
    ~SymbolTable();

    /*
    0=variables
    1=int
    2=constructor
    3=method
    4=holding all the class types
    */
    int type;
    Symbol* id;
    SymbolTable* parent;
    SymbolTable* returnType;

    int insert(Symbol s);
    int insert(Integer s);
    int insert(Method m);
    int insert(Constructor c);
    SymbolTable* lookup(Method m);
    SymbolTable* lookup(Constructor c);
    SymbolTable* lookup(Integer s);
    SymbolTable* lookup(Symbol s);
    void print(int& indent);

  private:
    unordered_map<string, vector<SymbolTable*>> symbols;
    unordered_map<Constructor, SymbolTable*> constructors;
    unordered_map<Integer, SymbolTable*> ints;
    unordered_map<Symbol, SymbolTable*> vars;
    vector<SymbolTable*> blocks;//used for conditional statements
};

#endif
