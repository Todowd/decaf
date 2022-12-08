/*
nodes.cpp
COSC4785
Tyler O'Dowd
12/5/22

Nodes that make up the syntax tree
*/
#include "nodes.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"

#include<iostream>
using std::cout;
using std::cerr;
using std::endl;

#include<string>
using std::string;
using std::to_string;

extern int ind;
extern Type* typeTable;

Node::Node(Node* Left, Node* Right, Node* Mid, Node* Last):
i(0), left(Left), right(Right), mid(Mid), last(Last) {
  next=nullptr;
  err=false;
  name="";
  error=false;
  reset();
}
Node::~Node() {
  if(left) {
		delete left;
  }
  if(right) {
	delete right;
  }
  if(mid) {
		delete mid;
  }
  if(last) {
    delete last;
  }
}
void Node::printError(string msg, string line) {
	cerr<<msg<<", line"<<myline<<": "<<line<<endl;
	for(int i=0; i<(mycol+msg.length()+to_string(myline).length()+8); i++) {
    cerr<<" ";
	}
	cerr<<"^"<<endl;
	//cerr<<msg<<", line"<<myline<<": "<<line<<endl;
}
bool Node::typeCheck() {
  bool er=false;
  if(type=="program") {
    //n is program
    Node* n=this;
    do {
      //table->declared has to be true, were going through it right now
      //and obviously it must exist too for the same reason
      //search is classdec
      Node* search=n->getLeft();
      string name=search->name;
      SymbolTable* table=typeTable->lookup(name);
      search=search->getRight();
      if(search) {
        //has no vardecs, constructors or methods
        if(search->name=="empty") {
          return error;
        }
        for(int i=0; i<search->name.length(); i++) {
          //search vardecs
          Node* p;
          if(i==0) {
            p=search->getLeft();
          }
          else if(i==1) {
            p=search->getMid();
          }
          else {
            p=search->getRight();
          }
          if((search->name[i])=='V') {
            er=(error||typeCheckVars(p, table));
          }
          //search constructors
          else if((search->name[i])=='C') {
            er=(error||typeCheckConsts(p, table));
          }
          //search methods
          else if((search->name[i])=='M') {
            er=(error||typeCheckMethods(p, table));
          }
          else {
            //how the heck???
            printError("unknown error", name);
            error=true;
          }
        }
      }
    } while(n->next);
  }
  else if(type=="block") {

  }
  return (error||er);
}
bool Node::typeCheckVars(Node* vars, SymbolTable* table) {
  bool error=false;
  do {
    //check if valid type
    error=(error||typeCheckVar(vars, table));
    vars=vars->next;
  } while(vars);
  return error;
}
bool Node::typeCheckVar(Node* var, SymbolTable* table) {
    string type=var->getLeft()->name;
    string name=var->getRight()->name;
    //same thing as !table->lookup(type)->declared
    if(!typeTable->exists(type)) {
      var->error=true;
      type+=" ";
      type+=name;
      type+=";";
      printError("unexpected type", type);
    }
    else if(checkValidVarType(type)) {
      var->error=true;
      type+=" "+name+";";
      printError("invalid l-value", type);
    }
    else if(
      (table->exists(name))//does it exist only in our current symbol table?
      &&(table->lookup(name)->id!=type) //does the type match?
      //&&(!table->lookup(name)->declared) //still need to check if it exists in this scope
    ) {
      var->error=true;
      type+=" ";
      type+=name;
      type+=";";
      name+=" already declared";
      printError(name, type);
    }
    return var->error;
}
bool Node::checkValidVarType(string str) {
  if(str=="null") {
    return true;
  }
  else if(str=="void") {
    return true;
  }
  return false;
}
bool Node::typeCheckConsts(Node* consts, SymbolTable* table) {
  return error;
}
bool Node::typeCheckMethods(Node* methods, SymbolTable* table) {
  return error;
}
int Node::getInt() const {
  return i;
}
string Node::getString() const {
  return str;
}
void Node::set(const char *v) {
  str=v;
}
void Node::set(const string s) {
  str=s;
}
void Node::set(int n) {
  i=n;
}

void Node::reset() {
  myline=mycol=1;
  str.clear();
}

void Node::setLeft(Node* p) {
  left=p;
  return;
}

void Node::setRight(Node* p) {
  right=p;
  return;
}

void Node::setMid(Node* p) {
  mid=p;
  return;
}
void Node::setLast(Node* p) {
  last=p;
  return;
}

void Node::setNext(Node* p) {
  if(next==0) next=p;
  else next->setNext(p);
  return;
}

Node* Node::getLeft() {
  return left;
}

Node* Node::getMid() {
  return mid;
}

Node* Node::getLast() {
  return last;
}

Node* Node::getRight() {
  return right;
}

Node* Node::getNext() {
  return next;
}

void Node::print() {
  indent(ind);
  ind++;
  cout<<str<<endl;
  if(left) {
	left->print();
  }
  if(mid) {
	mid->print();
  }
  if(right) {
	right->print();
  }
  if(last) {
	last->print();
  }
  ind--;
  if(next) {
    next->print();
  }
  return;
}
//bool Node::operator==(void) {
bool Node::isErr() {
	return err;
}

//NodeINT
NodeINT::NodeINT(int n) {
  i=n;
};
void NodeINT::print() {
  indent(ind);
  cout<<"NUMBER -> "<<i<<"\n";
  return;
}

//NodeNOSPACE
NodeNOSPACE::NodeNOSPACE(Node* n) {
	left=n;
};
void NodeNOSPACE::print() {
  indent(ind);
  ind++;
  cout<<str<<" ";
  if(left) {
	left->print();
  }
  if(mid) {
	mid->print();
  }
  if(right) {
	right->print();
  }
  if(last) {
	last->print();
  }
  ind--;
  if(next) {
    next->print();
  }
  return;

}

//NodeERROR
NodeERROR::NodeERROR() {
	err=true;
};
void NodeERROR::print() {
  return;
}

void indent(int n) {
	for(int i=0; i<n; i++) {
		cout<<"  ";
	}
}
