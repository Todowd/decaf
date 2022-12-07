/*
Entry.cpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold variable data
*/

#include<iostream>
using std::cout;
using std::endl;

#include "Entry.hpp"
#include "SymbolTable.hpp"

Entry::Entry() {
  id="#";//because not valid id
  type=nullptr;
}

Entry::Entry(string i, SymbolTable* t, int dim) {
  id=i;
  type=t;
  dims=dim;
}

bool Entry::equals(Entry* e) {
  return ((dims==e->dims)&&(type==e->type)&&(id==e->id));
}

string Entry::getName() {
  return id.substr(0, id.find("#", 0));
}

void Entry::print(int n) {
  in(n);
  int start=id.find("#", 0);
  cout<<id.substr(0, start)<<": ";
  //value#parameter_2 where "2" is order
  if(start<id.npos) {
    start=id.find("_", start);
    cout<<"Parameter "<<id.substr(start+1, id.npos)<<": ";
  }
  cout<<type->id;
  printDims();
  cout<<endl;
}
void Entry::in(int n) {
  for(int i=0; i<n; i++) {
    cout<<"  ";
  }
}

void Entry::printDims() {
  for(int i=0; i<dims; i++) {
    cout<<"[]";
  }
}
