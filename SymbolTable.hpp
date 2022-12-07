/*
SymbolTable.hpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold type symbol tables
*/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

class Entry;

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

#include<string>
using std::string;

class SymbolTable {
  public:
    SymbolTable(SymbolTable* p);
    ~SymbolTable();

    string id;
    int value;
    SymbolTable* parent;
    bool declared;
    SymbolTable* returnType;

    SymbolTable* lookup(string name);
    bool insert(string name, SymbolTable* t);
    bool insert(string name, Entry* e);
    SymbolTable* lastCall();
    void print();
    void print(int n);
    bool exists(string str);
		void addParamToName();
  protected:
    void in(int n);
  private:
    unordered_map<string, Entry*> vars;
    unordered_map<string, SymbolTable*> scope;
};


#endif
