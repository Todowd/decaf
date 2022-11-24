#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

class SymbolTable:public BlockTable {
  public:
    SymbolTable();
    SymbolTable(Symbol i);

    Symbol id;

    int insert(Method m);
    int insert(Constructor c);
    BlockTable* lookup(Method m);
    BlockTable* lookup(Constructor c);
    BlockTable* lookup(Symbol s);

    void print(int& indent);

  private:
    unordered_map<Method, /*vector<*/BlockTable*/*>*/> methods;
    unordered_map<Constructor, /*vector<*/BlockTable*/*>*/> constructors;
};

#endif
