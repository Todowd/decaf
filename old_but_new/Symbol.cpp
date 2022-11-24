#include "Symbol.hpp"

//GENERAL VARIABLE SYMBOL TYPE
Symbol::Symbol() {
  id=".";//because not valid id
  callup=nullptr;
}

Symbol::Symbol(string i) {
  id=i;
}
int Symbol::getDims() {
  return dimSize.size();
}
Symbol* Symbol::getDimSize(int i) {
  return dimSize[i];
}
void Symbol::setDimSize(int i, Symbol* v) {
  dimSize[i]=v;//hope you know what youre doing lol
  return;
}

//INTEGER SYMBOL TYPE
Integer::Integer(string i):Symbol(i) {
}
void Integer::setValue(int v) {
  val=v;
  return;
}
int Integer::getValue() {
  return val;
}

//CONSTRUCTOR SYMBOL TYPE
Constructor::Constructor(string i):Symbol(i) {
}
Symbol* Constructor::getParam(int i) {
  return &params[i];
}
int Constructor::getParamCount() {
  return params.size();
}
void Constructor::addParam(Symbol s) {
  params.push_back(s);
  return;
}

//METHOD SYMBOL TYPE
Method::Method(string i):Symbol(i) {
}
Symbol* Method::getParam(int i) {
  return &params[i];
}
int Method::getParamCount() {
  return params.size();
}
void Method::addParam(Symbol s) {
  params.push_back(s);
  return;
}
