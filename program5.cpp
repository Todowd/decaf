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

yyFlexLexer scanner;
Node *tree;
int ind;

int main() {
  tree=nullptr;
  ind=0;
  yyparse();
  tree->print();
  cout<<endl;
  return 0;
}
