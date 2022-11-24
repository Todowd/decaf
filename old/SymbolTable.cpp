#include "SymbolTable.h"

SymbolTable::SymbolTable() {
  id=Symbol();
  parent=nullptr;
}
SymbolTable::SymbolTable(Symbol* i, SymbolTable* p=nullptr) {
  parent=p;
  id=i;
}

int SymbolTable::insert(Method m);
int SymbolTable::insert(Constructor c) {
  if(lookup(c)) {
    return -1;
  }
  constructors[c]=new BlockTable()
  return 0;
}
SymbolTable* SymbolTable::lookup(Method m) {
  if(methods.count(m)) {
    return methods[m];
  }
  return nullptr;
}
BlockTable* SymbolTable::lookup(Constructor c) {
  if(constructors.count(c)>0) {
    return constructors[c];
    /*ONLY NEEDED IF the it.first() DOESNT WORK RIGHT
    bool found=false;
    int r=-1;
    for(auto it=constructors.begin(); it!=constructors.end(); ++it) {
      if(it->first().getParamCount()==c.getParamCount()) {
        for(int i=0; i<c.getParamCount(); i++) {
          if(c.getParam(i)->id!=it->first().getParam(i)) {
            found=false;
            break;
          }
        }
      }
      if(found) {
        return it->second();
      }
    }*/
  }
  return nullptr;
}
BlockTable* SymbolTable::lookup(Symbol s) {
  BlockTable* t=BlockTable::lookup(s);//do an cast upon assignment
  if(t!=nullptr) {
    return t;
  }
  Method* m=s;
  t=lookup(m);
  if(t!=nullptr) {
    return t;
  }
  Constructor* c=s;
  return lookup(c);
}

void SymbolTable::print(int& indent) {
  BlockTable::print(indent);
  if(indent<0) {
    return;
  }
  indent+=2;
  for(auto it=constructors.begin(); it!=constructors.end(); ++it) {
    for(int i=0; i<indent; i++) {
      cout<<" ";
    }
    for(int i=0; i<it->second().size(); i++) {
      cout<<it->first().id<<" method_type "<<it->first().returnType->id.id<<" <- ";
      for(int j=0; j<it->first().getParamCount(); j++) {
        cout<<it->first().getParam(j)->type->id.id<<" ";
      }
      cout<<end;
      it->second()[i]->print(indent);
    }
  }
  for(auto it=methods.begin(); it!=methods.end(); ++it) {
    for(int i=0; i<indent; i++) {
      cout<<" ";
    }
    for(int i=0; i<it->second().size(); i++) {
      cout<<it->first().id<<" method_type "<<it->first().returnType->id.id<<" <- ";
      for(int j=0; j<it->first().getParamCount(); j++) {
        cout<<it->first().getParam(j)->type->id.id<<" ";
      }
      cout<<end;
      it->second()[i]->print(indent);
    }
  }
  indent-=2;
}
