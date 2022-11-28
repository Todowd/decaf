#include<iostream>
using std::cout;
using std::endl;

#include "SymbolTable.hpp"

extern SymbolTable* root;

SymbolTable::SymbolTable(SymbolTable* p) {
  parent=p;
  next=nullptr;
  type=nullptr;
  value=0;
}

//insert whole SymbolTable
int SymbolTable::insert(string name, SymbolTable* t) {
  if(vars.count(name)>0) {
    return -2;
  }
  if(scope.count(name)>0) {
    //check for methods/constructor by same name
    SymbolTable* checker=scope[name];
    while(checker) {
      if(checker->type) {
        if(checker->type->equals(t->type)) {
          return -1;
        }
      }
      else {
        //the type doesnt exist in the SymbolTable, thats a problem
        return -3;
      }
    }
  }
  scope[name]=t;
  return 0;
}
//insert a whole Entry
int SymbolTable::insert(string name, Entry* e) {
  if(vars.count(name)>0) {
    return -2;
  }
  if(scope.count(name)>0) {
    return -1;
  }
  vars[name]=e;
  return 0;
}
//insert variable
int SymbolTable::insert(string name, string type, int dims) {
  if(vars.count(name)>0) {
    return -1;
  }
  else if(scope.count(name)>0) {
    return -2;
  }
  vars[name]=new Entry(name, type, dims);
  return 0;
}
//insert constructor/method
int SymbolTable::insert(string name, Type* t) {
  if(vars.count(name)>0) {
    return -2;
  }
  //is constructor
  if(name==id) {
    //if there is a return value, and constructor thats an Error
    if(t->rtn) {
      return -3;
    }
  }
  SymbolTable* loc=this;
  if(scope.count(name)>0) {
    //check for methods/constructor by same name
    SymbolTable* checker=scope[name];
    while(checker) {
      if(checker->type) {
        if(checker->type->equals(t)) {
          return -1;
        }
      }
      else {
        //the type doesnt exist in the SymbolTable, thats a problem
        return -4;
      }
    }
    loc=checker;
  }
  loc->next=new SymbolTable(this);
  loc->next->id=name;
  loc->next->type=t;
  return 0;
}

//I recognize this is recursive, and if there were too many layers it would
//need to be done with a loop, but were not dealing with anything that intense
SymbolTable* SymbolTable::lookup(string name) {
  if(scope.count(name)>0) {
    return scope[name];
  }
  if(vars.count(name)>0) {
    //the var type couldnt be of something defined in the class, but could be
    //the class the var is defined in, hence the parent->lookup
    //we also want the type of the var, and give that SymbolTable back
    return parent->lookup(vars[name]->type);
  }
  if(parent==nullptr) {
    return nullptr;
  }
  return parent->lookup(name);
}

bool SymbolTable::exists(string name) {
  return ((vars.count(name)>0)||(scope.count(name)>0));
}

void SymbolTable::print() {
  cout<<"<root table>"<<endl;
  int indent=2;
  for(auto it=scope.begin(); it!=scope.end(); ++it) {
    in(indent);
    cout<<it->first<<endl;
    indent+=2;
    it->second->print(indent);
    indent-=2;
  }
  return;
}
void SymbolTable::print(int& n) {
  in(n);
  int t=0;
  cout<<"<symbol table>: ";
  if(parent==root) {
    cout<<"<class> "<<id<<endl;
  }
  else if(type->rtn==nullptr) {
    cout<<"<constructor> "<<id<<endl;
    t=1;
  }
  else if(type) {
    //||(type->rtn=="VOID")||(type->rtn=="void"))
    cout<<"<method> "<<id<<endl;
    t=2;
  }
  else {//if((id==".")||(type==nullptr)) {
    cout<<"<block> "<<endl;
    t=3;
  }
  n+=2;
  //parameters
  if((t==1)||(t==2)) {
    Entry* tmp;
    for(int i=0; i<type->getParamCount(); i++) {
      tmp=type->getParam(i);
      in(n);
      cout<<tmp->id<<": parameter "<<i<<": "<<tmp->type<<endl;
    }
  }
  //variables
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    in(n);
    cout<<it->first<<": "<<it->second->type<<" <-- "<<it->second->dims<<endl;
  }
  //methods and constructors
  for(auto it=scope.begin(); it!=scope.end(); ++it) {
    Type* tmp=it->second->type;
    //its a method or constructor, but not a block
    if(tmp) {
      in(n);
      cout<<it->first<<": ";
      //has a return type, so much be a method
      if(tmp->rtn) {
        cout<<"method: "<<tmp->rtn->type;
        //cout<<"method: "<<tmp->rtn->id;
        cout<<" <-- "<<(tmp->getParamCount());
      }
      //if not method, then constructor
      else {
        cout<<"constructor:  <-- "<<tmp->getParamCount();
      }
      cout<<endl;
    }
  }
  //print underlying SymbolTable 's
  for(auto it=scope.begin(); it!=scope.end(); ++it) {
    SymbolTable* t=it->second;
    t->print(n);
    while(t->next) {
      t=t->next;
      t->print(n);
    }
  }
  n-=2;
}

void printDims(int n) {
  for(int i=0; i<n; i++) {
    cout<<"[]";
  }
}
void in(int n) {
  for(int i=0; i<n; i++) {
    cout<<" ";
  }
}












