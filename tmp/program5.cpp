/*
program3.cpp
COSC4785
Tyler O'Dowd
10/24/22

main program to run
*/

#include<iostream>
using std::cout;
using std::endl;

#include<fstream>
using std::ifstream;
using std::ofstream;

#include<string>
using std::string;

#include<FlexLexer.h>

#include "nodes.hpp"
#include "program5.tab.hpp"
#include "SymbolTable.hpp"
#include "Entry.hpp"
#include "Type.hpp"

yyFlexLexer scanner;
Node *tree;
int ind;
SymbolTable* root;
SymbolTable* super;
SymbolTable* table;
SymbolTable* callup;
SymbolTable* blk;
SymbolTable* upper;
Entry* symbol;
Entry* ret;
Type* prms;

int main() {
  tree=nullptr;
  root=new SymbolTable(nullptr);
  super=root;
  table=nullptr;
  callup=nullptr;
  blk=nullptr;
  upper=new SymbolTable(nullptr);
  symbol=nullptr;
  prms=nullptr;
  ret=nullptr;
  /*
  //Im just going to put the primatives into the table
  SymbolTable* tmp=new SymbolTable(root);
  tmp->id="int";
  root->insert(tmp);
  tmp=new SymbolTable(root);
  tmp->id="void";
  root->insert(tmp);
  */
  ind=0;
  yyparse();
  //tree->print(); //not needed after program4
  //cout<<endl;
  //root->print();
  upper->id="FUCK";
  upper->print();
  return 0;
}
