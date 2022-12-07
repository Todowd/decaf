/*
SymbolTable.cpp
COSC4785
Tyler O'Dowd
12/5/22

used to hold scope
*/
#include<iostream>
using std::cout;
using std::endl;

#include<vector>
using std::vector;

#include "SymbolTable.hpp"
#include "Entry.hpp"
//#include "Entry.hpp"

extern SymbolTable* root;
extern SymbolTable* upper;

SymbolTable::SymbolTable(SymbolTable* p) {
  parent=p;
  value=0;
  id="";
  declared=false;
  returnType=nullptr;
}

SymbolTable::~SymbolTable() {
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    if(it->second) {
      delete it->second;
    }
  }
  //returnType exists in Type and will be deleted
  //parent exists in Type and will be deleted
}

bool SymbolTable::insert(string str, SymbolTable* sym) {
  if(exists(str)) {
    return false;
  }
  scope.emplace(str, sym);
  return true;
}

bool SymbolTable::insert(string str, Entry* sym) {
  if(exists(str)) {
    return false;
  }
  vars.emplace(str, sym);
  return true;
}

SymbolTable* SymbolTable::lookup(string str) {
  unordered_map<string, SymbolTable*>::const_iterator it=scope.find(str);
  if(it==scope.end()) {
    unordered_map<string, Entry*>::const_iterator it2=vars.find(str);
    if(it2==vars.end()) {
      if(parent==nullptr) {
        return nullptr;
      }
      return parent->lookup(str);
    }
    return it2->second->type;
  }
  return it->second;
}

bool SymbolTable::exists(string name) {
  return ((vars.count(name)>0)||(scope.count(name)>0));
}

void SymbolTable::addParamToName() {
  string tmp="";
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    //length(#parameter)=10
    if(it->first.substr(it->first.find("#", 0), 10)=="#parameter") {
      tmp=it->second->type->id.substr(0, it->second->type->id.find("#", 0))+tmp;
      tmp="_"+tmp;
    }
  }
  id+=tmp;
}

void SymbolTable::print(int n) {
  in(n);
  cout<<"<symbol table>: ";
  if(parent==nullptr) {
    cout<<"<class> "<<id<<endl;
  }
  else if(id.find("#const", 0)<id.npos) {
  //else if(parent->id==id.substr(0, id.find("#const", 0))) {
    cout<<"<constructor> "<<id.substr(0, id.find("#const", 0))<<endl;
  }
  else if(id.find("#method", 0)<id.npos) {
    cout<<"<method> "<<id.substr(0, id.find("#method", 0))<<endl;
  }
  else {
    cout<<"<block> "<<endl;
  }
  n+=1;
  //variables
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    it->second->print(n);
  }
  //methods and constructors
  for(auto it=scope.begin(); it!=scope.end(); ++it) {
//std::cerr<<it->first<<endl;
    //check if its a block
    if(it->first.find("#block", 0)==it->first.npos) {
      in(n);
      //look to see if constructor or method
      int f=it->second->id.find("#const", 0);
      if(f<it->second->id.npos) {
        cout<<it->second->id.substr(0, f)<<": Constructor: ";
        //determine if has parameters
        if(it->second->id.find("_", f)!=it->second->id.npos) {
          //get only the params
          string tmp=it->second->id.substr(
            it->second->id.find("_", f),
            it->second->id.npos
          );
          tmp+="_";
          while(tmp!="_") {
            int helper=tmp.find("_", 1);
            cout<<tmp.substr(1, helper-1)<<" ";
            tmp=tmp.substr(helper, tmp.npos);
          }
        }
        //print the param count
        cout<<"<-- "<<it->second->value<<endl;
      }
      else {
        f=it->second->id.find("#method", 0);
        cout<<it->second->id.substr(0, f)<<": Method: ";
        //determine if has parameters
        if(it->second->id.find("_", f)!=it->second->id.npos) {
          //get only the params
          string tmp=it->second->id.substr(
            it->second->id.find("_", f),
            it->second->id.npos
          );
          tmp+="_";
          while(tmp!="_") {
            int helper=tmp.find("_", 1);
            cout<<tmp.substr(1, helper-1)<<" ";
            tmp=tmp.substr(helper, tmp.npos);
          }
        }
        //print the param count
        cout<<"<-- "<<it->second->value<<endl;
      }
    }
  }
  //print underlying SymbolTable 's
  for(auto it=scope.begin(); it!=scope.end(); ++it) {
    it->second->print(n);
  }
  n-=1;
}

void SymbolTable::in(int n) {
  for(int i=0; i<n; i++) {
    cout<<"  ";
  }
}












