#ifndef MASTERTABLE_H
#define MASTERTABLE_H

#include<unordered_map>
using std::unordered_map;

class MasterTable {
  public:
    MasterTable();
    int insert(Symbol s);
    SymbolTable* lookup(Symbol s);

  private:
    unordered_map<string, SymbolTable*> classes;
};

#endif
