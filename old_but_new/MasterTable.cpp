#include "SymbolTable.h"
#include "Symbol.h"
#include "MasterTable.h"

MasterTable::MasterTable() {

}

SymbolTable* MasterTable::insert(Symbol s) {
  SymbolTable* t=lookup(s);
  if(t==nullptr) {
    classes[s]=new SymbolTable(s);
  }
  return classes[s];
}

SymbolTable* MasterTable::lookup(Symbol s) {
  if(classes.count(s.id)>0) {
    return classes[s.id];
  }
  return nullptr;
}
