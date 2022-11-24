#ifndef MASTERTABLE_H
#define MASTERTABLE_H

#include<unordered_map>
using std::unordered_map;

//#include "Symbol.h"

class MasterTable {
  public:
    MasterTable();
    int insert(Symbol s);
    SymbolTable* lookup(Symbol s);

  private:
    unordered_map<Symbol, SymbolTable*> classes;
};

#endif
