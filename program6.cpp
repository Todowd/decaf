/*
program6.cpp
COSC4785
Tyler O'Dowd
12/5/22

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
#include "program6.tab.hpp"
#include "SymbolTable.hpp"
#include "Entry.hpp"
#include "Type.hpp"

yyFlexLexer scanner;
Node *tree;
int ind;
SymbolTable* table;
SymbolTable* sub;
Entry* symbol;
Type* typeTable;
string value;
string typeValue;

int main() {
  tree=nullptr;
  symbol=nullptr;
  typeTable=new Type();
  //manually enter void
  table=new SymbolTable(nullptr);
  table->id="void";
  table->declared=true;
  typeTable->insert("void", table);
  //manually enter int
  table=new SymbolTable(nullptr);
  table->id="int";
  table->value=0;
  table->declared=true;
  typeTable->insert("int", table);
  //manually enter void
  table=new SymbolTable(nullptr);
  table->id="null";
  table->value=0;
  table->declared=true;
  typeTable->insert("null", table);
  table=nullptr;

  ind=0;
  yyparse();
  //tree->print(); //not needed after program4
  //cout<<endl;
  //root->print();
  typeTable->print(ind);
  return 0;
}
