#include "Symbol.h"

//GENERAL VARIABLE SYMBOL TYPE
Symbol::Symbol() {
  id=".";//because not valid id
  type=nullptr;//because this shouldnt exist
}

Symbol::Symbol(string i, SymbolTable* t) {
  id=i;
  type=t;
}
int Symbol::getDims() {
  return dimSize.size();
}
int Symbol::getDimSize(int i) {
  return dimSize[i];
}
void Symbol::setDimSize(int i, int v) {
  dimSize[i]=v;//hope you know what youre doing lol
  return;
}

//INTEGER SYMBOL TYPE
Integer::Integer(string i):Symbol(i) {
  //Symbol(i);
  //id=i;
  val=0;
  //dimSize(0);
  dimSize=vector(0);
}
void Integer::setValue(int v) {
  val=v;
  return;
}
int Integer::getValue() {
  return val;
}

//CONSTRUCTOR SYMBOL TYPE
Constructor::Constructor(string i, vector<Symbol*> p):Symbol(i) {
  params=p;
}
Constructor::~Constructor() {
  while(params.size()>0) {
    Symbol* s=params.front();
    if(s) {
      delete s;
    }
  }
}
Symbol* Constructor::getParam(int i) {
  return params[i];
}
int Constructor::getParamCount() {
  return params.size();
}

//METHOD SYMBOL TYPE
Method::Method(string i, vector<Symbol*> p, SymbolTable* t):Symbole(i) {
  params=p;
  returnType=t;
}
Method::~Method() {
  while(params.size()>0) {
    Symbol* s=params.front();
    if(s) {
      delete s;
    }
  }
}
Symbol* Method::getParam(int i) {
  return params[i];
}
int Method::getParamCount() {
  return params.size();
}
