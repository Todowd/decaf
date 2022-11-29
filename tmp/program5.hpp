/*
program.hpp
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
#include "SymbolTable.hpp"

extern int col;
extern int line;
extern std::string err;
extern Node* tree;
extern SymbolTable* root;
extern std::string errType;
extern SymbolTable* super;
extern SymbolTable* table;
extern SymbolTable* blk;
extern SymbolTable* callup;
extern SymbolTable* upper;
extern Entry* symbol;
extern Entry* ret;
extern Type* prms;

#endif
