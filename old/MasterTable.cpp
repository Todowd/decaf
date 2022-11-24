#include "MasterTable.h"

MasterTable::MasterTable() {

}

int MasterTable::insert(Symbol s) {
  SymbolTable* t=lookup(s);
  if(t==nullptr) {
    classes[s.id]=new SymbolTable(s);
    return 0;//added successfully
  }
  return -1;//already exists
}

SymbolTable* MasterTable::lookup(Symbol s) {
  if(classes.count(s.id)>0) {
    return classes[s.id];
  }
  return nullptr;
}

SymbolTable* MasterTable::get(Symbol s) {

}
