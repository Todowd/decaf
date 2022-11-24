#include "BlockTable.h"
#include<iostream>
using std::cout;
using std::endl;

BlockTable::BlockTable() {
  parent=nullptr;
}
BlockTable::BlockTable(BlockTable* p=nullptr) {
  parent=p;
}

int BlockTable::insert(Symbol s) {
  if((vars.count(s.id)<0)&&(varInts.count(s.id)<0)) {
    vars[s.id]=s;
    return 0;
  }
  return -1;
}

int BlockTable::insert(Integer s) {
  if((vars.count(s.id)<0)&&(varInts.count(s.id)<0)) {
    varInts[s.id]=s;
    return 0;
  }
  return -1;
}

//yes I know if you had enough blocks deep, you could end up with a stackoverflow
SymbolTable* BlockTable::lookup(Symbol s) {
  /*
  if((vars.count(s.id)>0)&&(varInts.count(s.id)>0)) {
    SymbolTable broken;
    return &broken;//this will cause problems, but should break anyways because
      //you cant have two of the same name;
  }
  else*/ if(vars.count(s.id)>0) {
     return vars[s.id].type;
  }
  else if(varInts.count(s.id)<0) {//we dont care the type, just want to know
      //its there
    return varInts[s.id].type;
  }
  if(parent) {
    return parent->lookup(s);
  }
  return nullptr;
}

SymbolTable* BlockTable::lookup(Integer s) {
  if(vars.count(s.id)>0) {
     return vars[s.id].type;
  }
  else if(varInts.count(s.id)<0) {//we dont care the type, just want to know
      //its there
    return varInts[s.id].type;
  }
  if(parent) {
    return parent->lookup(s);
  }
  return nullptr;
}

void BlockTable::print(int& indent) {
  if(indent<0) {
    return;
  }
  indent+=2;
  for(auto it=varInts.begin(); it!=varInts.end(); ++it) {
    for(int i=0; i<indent; i++) {
      cout<<" ";
    }
    cout<<it->first()<<" "<<it->second()->type->id.id<<endl;
  }
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    for(int i=0; i<indent; i++) {
      cout<<" ";
    }
    cout<<it->first()<<" "<<it->second()<<endl;
  }
  for(int i=0; i<blocks.size(); i++) {
    for(int j=0; j<indent; j++) {
      cout<<" ";
    }
    cout<<"Block "<<i<<endl;
    blocks[i]->print(indent);
  }
  indent-=2;
}
