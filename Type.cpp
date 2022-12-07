/*
type.cpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold type symbol tables
*/
#include "Type.hpp"

#include<iostream>
using std::cout;
using std::endl;

Type::Type() {
}

Type::~Type() {
  for(auto it=types.begin(); it!=types.end(); it++) {
    if(it->second) {
      delete it->second;
    }
  }
}

bool Type::insert(string str, SymbolTable* sym) {
  if(types.count(str)>0) {
    return false;
  }
  types.emplace(str, sym);
  return true;
}

SymbolTable* Type::lookup(string str) {
  unordered_map<string, SymbolTable*>::const_iterator it=types.find(str);
  if(it==types.end()) {
    SymbolTable* t=new SymbolTable(nullptr);
    t->id=str;
    types.emplace(str, t);
    return t;
  }
  return it->second;
}

void Type::print(int n) {
  cout<<"Type Table:"<<endl;
  n++;
  for(auto it=types.begin(); it!=types.end(); it++) {
    if(it->second->declared) {
      cout<<it->first<<endl;
      it->second->print(n);
    }
  }
}

void Type::indent(int n) {
  for(int i=0; i<n; i++) {
    cout<<"  ";
  }
}
