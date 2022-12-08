/*
type.hpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold type symbol tables
*/
#ifndef TYPE_H
#define TYPE_H

#include<unordered_map>
using std::unordered_map;

#include<string>
using std::string;

#include "SymbolTable.hpp"
//#include "Entry.hpp"

class Type {
  public:
    Type();
    ~Type();
    bool insert(string str, SymbolTable* sym);
    SymbolTable* lookup(string str);
    void print(int n);
    bool exists(string str);
  private:
    unordered_map<string, SymbolTable*> types;
    void indent(int n);
};

#endif
