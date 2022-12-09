/*
nodes.hpp
COSC4785
Tyler O'Dowd
12/5/22

Nodes that make up the syntax tree
*/
#ifndef NODES_HPP
#define NODES_HPP

#include<iostream>
using std::endl;
using std::ostream;
#include<string>
using std::string;

#include "SymbolTable.hpp"

class Node {
  public:
    Node(
		Node* Left=nullptr,
		Node* Right=nullptr,
		Node* Mid=nullptr,
		Node* Last=nullptr
	);
    virtual ~Node();
    int getInt() const;
    string getString() const;
    void set(const char* val);
    void set(const string str);
    void set(int n);
    void set(double d);
    void reset();
    void setLeft(Node* p);
    void setRight(Node* p);
    void setMid(Node* p);
    void setLast(Node* p);
    void setNext(Node* p);
    Node* getLeft();
    Node* getRight();
    Node* getNext();
    Node* getMid();
    Node* getLast();
    virtual void print();
    bool isErr();

    string name;
    string type;
    SymbolTable* block;
    bool error;
    void printError(string msg, string line);
    virtual bool typeCheck();
    static bool constructor;
    static int mainCount;

  protected:
    int myline;
    int mycol;
    int i;
    string str;
    Node* left;
    Node* right;
    Node* next;
    Node* mid;
    Node* last;
    bool err;
    //yeah the virtual is kinda unnecessary (see comment in nodes.cpp)
    virtual bool typeCheckVars(Node* vars, SymbolTable* table);
    virtual bool typeCheckVar(Node* vars, SymbolTable* table);
    virtual bool typeCheckConsts(Node* consts, SymbolTable* table);
    virtual string constLine(Node* f);
    virtual bool typeCheckMethods(Node* methods, SymbolTable* table);
    virtual bool checkValidVarType(string str);
    virtual string methodLine(Node* f);
    virtual bool typeCheckBlock();
    virtual bool typeCheckStmts(SymbolTable* table)
    virtual bool typeCheckStmt(SymbolTable* table)
    virtual bool typeCheckName(SymbolTable* table)
    virtual bool typeCheckExp(SymbolTable* table)
    string getNameLine();
    string getExpLine();
    string getArglistLine();
    string getNewexprLine();
    string getBracketexpsLine();
    string getMultibracketsLine();
};

class NodeINT: public Node {
  public:
    NodeINT(int n);
    virtual void print();
};
class NodeNOSPACE: public Node {
  public:
    NodeNOSPACE(Node* n=nullptr);
    virtual void print();
};
class NodeERROR: public Node {
  public:
    NodeERROR();
    virtual void print();
};

void indent(int n);

#endif
