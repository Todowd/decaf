#ifndef SYMBOL_H
#define SYMBOL_H

#include<vector>
using std::vector;

#include<string>
using std::string;

//#include "SymbolTable.hpp"

class Symbol {
  public:
    Symbol();
    Symbol(string i);
    string id;
    int getDims();
    Symbol* getDimSize(int i);
    void setDimSize(int i, Symbol* v);
    Symbol* callup;
  protected:
    vector<Symbol*> dimSize;
};

class Integer:public Symbol {
  public:
    Integer(string i);
    void setValue(int v);
    int getValue();
  private:
    int val;
};

class Constructor:public Symbol {
  public:
    Constructor(string i);
    Symbol* getParam(int i);
    int getParamCount();
    void addParam(Symbol s);
  private:
    vector<Symbol> params;
};

class Method:public Symbol {
  public:
    Method(string i);
    Symbol* getParam(int i);
    int getParamCount();
    void addParam(Symbol s);
  private:
    vector<Symbol> params;
};

#endif
