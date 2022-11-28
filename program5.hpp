/*
program.hpp
COSC4785
Tyler O'Dowd
9/30/22

allows the cpp and scanner to both use the string called value

*/

#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include<string>

#include "nodes.hpp"

extern int col;
extern int line;
extern std::string err;
extern Node* tree;
extern SymbolTable* root;
extern SymbolTable* tmp;
extern std::string errType;

#endif
