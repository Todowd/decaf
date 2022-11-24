//#include "Symbol.h"

#include "SymbolTable.hpp"
//#include<typeinfo>

#define VAR 0
#define INT 1
#define CONST 2
#define METH 3
#define CLASS 4

SymbolTable::SymbolTable() {
  id=nullptr;
  parent=nullptr;
  resultType=nullptr;
  type=-1;
}
SymbolTable::SymbolTable(Symbol* i, int t, SymbolTable* p) {
  parent=p;
  id=i;
  type=t;
}
SymbolTable::~SymbolTable() {
  if(returnType) {
    delete returnType;
  }
}

int SymbolTable::insert(Constructor c) {
  if(constructors.count(c)>0) {
    constructors[c]=new SymbolTable(c, CONST, this);
    return -1;
  }
  return 0;
}
int SymbolTable::insert(Method m) {
  if(methods.count(m)>0) {
    return -1;
  }
  methods[m]=new SymbolTable(m, METH, this);
  return 0;
}
int SymbolTable::insert(Integer s) {
  if(ints.count(s)>0) {
    return -1;
  }
  ints[s]=new SymbolTable(s, INT, this);
  return 0;
}
int SymbolTable::insert(Symbol s) {
  if(vars.count(s)>0) {
    return -1;
  }
  vars[s]=new SymbolTable(s, VAR, this);
  return 0;
}
SymbolTable* SymbolTable::lookup(Method m) {
  if(methods.count(m)>0) {
    return methods[m];
  }
  return nullptr;
}
SymbolTable* SymbolTable::lookup(Constructor c) {
  if(constructors.count(c)>0) {
    return constructors[c];
  }
  //there is no parent of the class, not doing any inheritance or anything here
  return nullptr;
}
SymbolTable* SymbolTable::lookup(Integer s) {
  if(ints.count(s)>0) {
    return ints[s];
  }
  if(parent) {
    if(type==CONST) {
      Constructor* c=id;
      for(int i=0; i<c->getParamCount(); i++) {
        if(c->getParam(i).id==s.id) {
          return c->getParam(i).type;
        }
      }
    }
    if(type==METH) {
      Method* m=id;
      for(int i=0; i<m->getParamCount(); i++) {
        if(c->getParam(i).id==s.id) {
          return m->getParam(i).type;
        }
      }
    }
    /*
    //parent is constructor check its parameters
    Constructor* c=new Constructor(".", vector<Symbol>());
    if(typeid(c)==typeid(parent)) {
      delete c;
      c=parent;
      for(int i=0; i<c->getParamCount(); i++) {
        if(s.id==c->getParam(i).id) {
          return parent;
        }
      }
    }
    //parent is method, check params
    Method* m=new Method(".", vector<Symbol>(), nullptr);
    if(typeid(m)==typeid(parent)) {
      delete m;
      m=parent;
      for(int i=0; i<m->getParamCount(); i++) {
        if(s.id==m->getParam(i).id) {
          return parent;
        }
      }
    }
    */
    //
    return parent->lookup(s);
  }
  //the integer type is not a class itself
  return nullptr;
}
SymbolTable* SymbolTable::lookup(Symbol s) {
  //we will assume that the Symbol, s, is a variable of some class type
  if(vars.count(s)>0) {
    return vars[s];
  }
  if(parent) {
    //parent is constructor check params
    if(type==CONST) {
      Constructor* c=id;
      for(int i=0; i<c->getParamCount(); i++) {
        if(c->getParam(i).id==s.id) {
          return c->getParam(i).type;
        }
      }
    }
    //parent is a method, check params
    if(type==METH) {
      Method* m=id;
      for(int i=0; i<m->getParamCount(); i++) {
        if(c->getParam(i).id==s.id) {
          return m->getParam(i).type;
        }
      }
    }
    return parent->lookup(s);
  }
  //not found anywhere
  return nullptr;
}

//NEEDS FIXED
void SymbolTable::print(int& indent) {
/*  BlockTable::print(indent);
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
  indent-=2;*/
}
