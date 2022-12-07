/*
program6.hpp
COSC4785
Tyler O'Dowd
9/30/22

allows the cpp and scanner to both use the string called value

*/

#ifndef PROGRAM5_HPP
#define PROGRAM5_HPP

#include<string>

#include "nodes.hpp"
#include "SymbolTable.hpp"
#include "Type.hpp"

extern int col;
extern int line;
extern std::string err;
extern Node* tree;
extern std::string errType;
extern SymbolTable* table;
extern SymbolTable* sub;
extern SymbolTable* current;
extern Entry* symbol;
extern Type* typeTable;
extern string value;
extern string typeValue;

#endif
