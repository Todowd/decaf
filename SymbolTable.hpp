#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

#include "Symbol.hpp"

class SymbolTable {
  public:
    struct optional {
      optional() {
        table=nullptr;
        var=nullptr;
      }
      vector<SymbolTable*>* table;
      Symbol* var;
    };

    SymbolTable(SymbolTable* p);/*
    void set(
                unordered_map<string, vector<SymbolTable*>> c
    );
    void set(
                unordered_map<string, Symbol*> p
    );
    void set(
                unordered_map<string, Symbol*> v
    );*/
    unordered_map<string, vector<SymbolTable*>> getChildren();
    unordered_map<string, Symbol*> getVars();
    unordered_map<string, Symbol*> getParams();

    string id;
    string returnType;
    SymbolTable* parent;

    int insert(string id);//class
    int insert(string id, string type, int dim);//variable
    int insert(string id, int count, Symbol** params, string rtn);//method
    int insert(string id, int count, Symbol** params);//constructor
    int insert(string id, SymbolTable* t);
    SymbolTable* insert();//block
    optional lookup(string id);//methods blocks and constructors
    void print();
    int getParamCount();
    Symbol* getParam(string s);
    Symbol* getVar(string str);
    vector<SymbolTable*>* getChild(string str);

  protected:
    void print(int& n);
  private:
    unordered_map<string, vector<SymbolTable*>> children;
    unordered_map<string, Symbol*> vars;
    unordered_map<string, Symbol*> params;
};

void printDims(int n);
void in(int n);

#endif
