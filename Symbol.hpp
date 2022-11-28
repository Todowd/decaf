#ifndef SYMBOL_H
#define SYMBOL_H

#include<string>
using std::string;

class Symbol {
  public:
    Symbol();
    Symbol(string i, string t, int dims=0);
    string id;
    string type;
    //SymbolTable* type;
    int dims;
};

#endif
