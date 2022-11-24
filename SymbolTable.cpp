#include<iostream>
using std::cout;
using std::endl;

#include "SymbolTable.hpp"

SymbolTable::SymbolTable(SymbolTable* p) {
  parent=p;
  returnType="";
}

//I know its not exactly a wrapper here, but it was the quickest way
//and it doesnt seem like it was where you really wanted things to be
//like a wrapper in the use that it has in the parser
void SymbolTable::set(
  unordered_map<string, vector<SymbolTable*>> c
) {
  chilren=c;
}
void SymbolTable::set(
  unordered_map<string, Symbol*> p
) {
  params=p;
}
void SymbolTable::set(
  unordered_map<string, Symbol*> v
) {
  vars=v;
}
unordered_map<string, vector<SymbolTable*>> getChildren() {
  return children;
}
unordered_map<string, Symbol*> getVars() {
  return vars;
}
unordered_map<string, Symbol*> getParams() {
  return params;
}

int SymbolTable::getParamCount() {
  return params.size();
}

Symbol* SymbolTable::getParam(string s) {
  if(params.count(s)>0) {
    return params[s];
  }
  return nullptr;
}
Symbol* getVar(string str) {
  if(vars.count(str)>0) {
    return vars[str];
  }
  return nullptr;
}
vector<SymbolTable*> getChild(string str) {
  if(children.count(str)>0) {
    return children(str);
  }
  return nullptr;
}

int SymbolTable::insert(string id, string type, int dim) {
  if((children.count(id)>0)||(vars.count(id)>0)||(params.count(id)>0)) {
    return -1;
  }
  vars[id]=new Symbol(id, type, dim);
  return 0;
}
int SymbolTable::insert(string id, int count, Symbol** p, string rtn) {
  if((vars.count(id)>0)||(params.count(id)>0)) {
    return -1;
  }
  if(children.count(id)>0) {
    bool found=true;
    for(int i=0; i<children[id].size(); i++) {
      SymbolTable* t=children[id][i];
      if((t->returnType==rtn)&&(t->getParamCount()==count)) {
        found=true;
        for(int j=0; j<count; j++) {
          if(t->getParam(p[j]->id)) {
              if(!(t->getParam(p[j]->id)->type==p[j]->type)) {
                found=false;
                break;
              }
          }
          else {
            found=false;
            break;
          }
        }
      }
    }
    if(found) {
      return -1;
    }
  }
  SymbolTable* table=new SymbolTable(this);
  table->id=id;
  table->returnType=rtn;
  children[id].push_back(table);
  return 0;
}
int SymbolTable::insert(string id, int count, Symbol** p) {

  if((vars.count(id)>0)||(params.count(id)>0)) {
    return -1;
  }
  if(children.count(id)>0) {
    bool found=true;
    for(int i=0; i<children[id].size(); i++) {
      SymbolTable* t=children[id][i];
      if(t->getParamCount()==count) {
        found=true;
        for(int j=0; j<count; j++) {
          if(t->getParam(p[j]->id)) {
              if(!(t->getParam(p[j]->id)->type==p[j]->type)) {
                found=false;
                break;
              }
          }
          else {
            found=false;
            break;
          }
        }
      }
    }
    if(found) {
      return -1;
    }
  }
  SymbolTable* table=new SymbolTable(this);
  table->id=id;
  children[id].push_back(table);
  return 0;
}
int SymbolTable::insert() {
  SymbolTable* t=new SymbolTable(this);
  t->id="BLOCK";
  children["."].push_back(t);
  return 0;
}
int SymbolTable::insert(string id) {
  if(children.count(id)>0) {
    return -1;
  }
  SymbolTable* t=new SymbolTable(this);//this being like the root table
  t->id=id;
  children[id].push_back(t);
  return 0;
}
SymbolTable::optional SymbolTable::lookup(string id) {
  SymbolTable::optional o;
  if(vars.count(id)>0) {
    o.var=vars[id];
    o.table=nullptr;
  }
  else if(params.count(id)>0) {
    o.var=params[id];
    o.table=nullptr;
  }
  else if(children.count(id)>0) {
    o.var=nullptr;
    o.table=&children[id];
  }
  return o;
}
void SymbolTable::print() {
  cout<<"<root table>"<<endl;
  int indent=0;
  for(auto it=children.begin(); it!=children.end(); ++it) {
    cout<<"  "<<it.first()<<endl;
    indent+=4;
    it.second()[0]->print(indent);
    indent-=4;
  }
  return;
}

void SymbolTable::print(int& n) {
  in(n);
  cout<<"<symbol table>: ";
  if(n==4) {
    cout<<"<class>"<<id<<endl;
  }
  else if(id==".") {
    cout<<"<block>"<<endl;
  }
  else if(returnType=="") {
    cout<<"<constructor> "<<id<<endl;
  }
  else {
    cout<<"<method> "<<id<<endl;
  }
  n+=2;
  for(auto it=params.begin(); it!=params.end(); ++it) {
    in(n);
    cout<<it.first()<<": parameter: "it.second()->type<<endl;
  }
  for(auto it=vars.begin(); it!=vars.end(); ++it) {
    in(n);
    cout<<it.first()<<": "it.second()->type;
    printDims(it.second()->dims);
    cout<<endl;
  }
  for(auto it=children.begin(); it!=children.end(); ++it) {
    for(int i=0; i<it.second().size(); i++) {
      if(t->id==".") {
        cout<<"block"<<endl;
        continue;
      }
      cout<<it.first()<<": ";
      Symboltable* t=it.second()[i];
      if(t->returnType=="") {
        cout<<"constructor: "<<t->getParamCount()<<endl;
      }
      else {
        cout<<"method: "<<t->returnType<<" - "<<t->getParamCount()<<end;
      }
      cout<<it.second()
    }
  }
  for(auto it=children.begin(); it!=children.end(); ++it) {
    for(int i=0; i<it.second().size(); i++) {
        it.second()[i]->print(n);
    }
  }
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












