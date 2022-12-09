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
extern bool constructor;
extern int mainCount;

Node::Node(Node* Left, Node* Right, Node* Mid, Node* Last):
i(0), left(Left), right(Right), mid(Mid), last(Last) {
  next=nullptr;
  err=false;
  name="";
  error=false;
  constructor=false;
  mainCount=0;
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
	for(int i=0; i<(mycol+msg.length()+to_string(myline).length()+9); i++) {
    cerr<<" ";
	}
	cerr<<"^"<<endl;
	//cerr<<msg<<", line"<<myline<<": "<<line<<endl;
}
/*
Could I have created several node types, instead of one function calling many
others yes, yes I could have. But that would also require restructuring
everything else this works. If I wasnt on such a time crunch, it wouldve been
better to do it that way

Im just trying to recognize that although this is the way I did it, its not
the way I wouldve wanted to have done it.
*/
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
      //how idk but sure
      if(!table->declared) {
        printError("unexpected type "+table->id, "class "+table->id+"{");
        return true;
      }
      search=search->getRight();
      if(search) {
        //has no vardecs, constructors or methods
        if(search->name=="empty") {
          return error;
        }
        //if theres a mid, then it has all three
        else if(search->getMid()) {
          er=(error||typeCheckVars(search->getLeft(), table));
          er=(error||typeCheckConsts(search->getMid(), table));
          er=(error||typeCheckMethods(search->getRight(), table));
        }
        //there will be a left at this point whether there are one or two nodes
        else {
          if(search->name[0]=='V') {
            er=(error||typeCheckVars(search->getLeft(), table));
          }
          else if(search->name[0]=='C') {
            er=(error||typeCheckConsts(search->getLeft(), table));
          }
          else {
            er=(error||typeCheckMethods(search->getLeft(), table));
          }
          //just need the right nodes now
          if(search->name.length()==2) {
            if(search->name[1]=='V') {
              er=(error||typeCheckVars(search->getRight(), table));
            }
            else if(search->name[1]=='C') {
              er=(error||typeCheckConsts(search->getRight(), table));
            }
            else {
              er=(error||typeCheckMethods(search->getRight(), table));
            }
          }
        }
      }
      n=n->getNext();
    } while(n);
  }
  //constructor/method block with local vardecs and stmts
  else if((type=="localvardecs_stmts")||(type=="localvardecs")||(type=="stmts")) {
    //this as of in the keyword/operator is a block node
    er=typeCheckBlock();
    //create function that could be recursively called to type check
    //first part of block is easy with vardecs (maybe?)
    //then this is were all the exp and stmts start coming into play
      //there will be many ifs/elses
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
  bool err=consts->error;
  constructor=true;
  //check the constructor name matches the class type
  string name=consts->name;
  Node* params=consts->getMid()->getLeft();
  if(name.substr(0, name.find("#", 0))!=table->id) {
    printError("unexpected method", constLine(consts));
  }
  table=table->lookup(name);
  if(!table->declared) {
    printError("unexpected type", table->id);
    return true;
  }
  //check params
  if(consts->getMid()->type!="empty") {
    err=(err||params->error||typeCheckVars(params, table));
  }
  //constructor name doesnt repeat, checked in parser
  //check rest of constructor block
  consts->getRight()->block=table;//just incase it wasnt already
  return (err||consts->getRight()->typeCheck());
}
string Node::constLine(Node* f) {
  string name=f->getLeft()->name;
  name=name.substr(0, name.find("#", 0));
  name+="(";
  Node* n=f->getMid();
  if(n->type!="empty") {
    n=n->getLeft();
    name+=n->type+" "+n->name;
    while(n) {
      n=n->getNext();
      name+=", "+n->type+" "+n->name;
    }
  }
  name+=")";
  return name;
}
bool Node::typeCheckMethods(Node* methods, SymbolTable* table) {
  bool err=false;
  constructor=false;
  //check the return type exists
  Node* n=methods->getLeft();
  if(!typeTable->exists(n->type)) {
    string tmp="unexpected type "+n->type;
    printError(tmp, methodLine(methods));
    methods->getLeft()->error=false;
    err=true;
  }
  //we already know it exists, and wasnt doubled from parser
  //check params
  string name=methods->name;
  if(name=="main") {
    mainCount++;
    if(mainCount>1) {
      printError("main already declared", methodLine(methods));
      return true;
    }
    else if(mainCount==1) {
      if((n->type!="void")||(n->type!="int")) {
        printError("invalid return type for main", methodLine(methods));
        return true;
      }
    }
  }
  Node* params=methods->getRight()->getLeft();
  table=table->lookup(name);
  if(!table->declared) {
    printError("unexpected type", table->id);
    return true;
  }
  //check params
  if(methods->getRight()->type!="empty") {
    err=(err||params->error||typeCheckVars(params, table));
  }
  //method name doesnt repeat, checked in parser
  //check rest of method block
  methods->getLast()->block=table;//just incase it wasnt already
  return (err||methods->getLast()->typeCheck());
}
string Node::methodLine(Node* f) {
  string name=f->getLeft()->type;
  name+=f->name;//name+=f->getMid()->name;
  name=name.substr(0, name.find("#", 0));
  name+="(";
  Node* n=f->getRight();
  if(n->type!="empty") {
    n=n->getLeft();
    name+=n->type+" "+n->name;
    while(n) {
      n=n->getNext();
      name+=", "+n->type+" "+n->name;
    }
  }
  name+=")";
  return name;
}
bool Node::typeCheckBlock() {
  bool err=false;
  if(type=="empty") {
    return err;
  }
  if((type=="localvardecs_stmts")||(type=="localvardecs")) {
    //left is a localvardec
    err=typeCheckVars(left, block);
    //if right exists its the stmts
    if(right) {
      err=(err||right->typeCheckStmts(block));
    }
  }
  else {
    //left is an stmts
    err=left->typeCheckStmts(block);
  }
  return err;
}
bool Node::typeCheckStmts(SymbolTable* table) {
  bool err=typeCheckStmt(table);
  if(next) {
    err=(err||next->typeCheckStmts(table));
  }
  return err;
}
bool Node::typeCheckStmt(SymbolTable* table) {
  bool err=false;
  //no error with just a semi
  if(type=="semi") {
    return false;
  }
  else if(type=="assign") {
    err=(err||left->typeCheckName(table));
    err=(err||right->typeCheckExp(table));
    string tmp=left->evalNameID(table);
    if(tmp=="") {
      return true;
    }
    SymbolTable* leftVal=left->evalNameTable(table);
    SymbolTable* rightVal=right->evalExpResult(table);
    if(rightVal==nullptr) {
      printError("invalid expression", right->getExpLine());
      return true;
    }
    if(leftVal==nullptr) {
      return true;
    }
    //if theyre of the same type then we should have the same SymbolTable's
    if(leftVal!=rightVal) {
      error=true;
      string line=left->getNameLine();
      line+="=";
      line+=right->getExpLine();
      printError("Error mismatching types", line);
      return true;
    }
    if((rightVal->id=="void")||(right->name=="int")) {
      string line=left->getNameLine();
      line+="=";
      line+=right->getExpLine();
      printError("invalid rvalue", line);
      return true;
    }
    return false;
  }
  else if(type=="method_call") {
    if(left->typeCheckName(table)) {
      return true;
    }
    string str=left->evalNameID(table);
    if(str=="") {
      return true;
    }
    if(!typeTable->exists(str)) {
      str=left->getNameLine();
      str+="("+right->getArglistLine()+")";
      printError("unexpected constructor", str);
      return true;
    }
    SymbolTable* belongsto=left->evalNameTable(table);
    str=left->name+"#"+right->argListTypes(table);
    SymbolTable* tmp=belongsto->lookup(str);
    if((!(tmp))||(!tmp->declared)) {
      str=left->getNameLine();
      str+="("+right->getArglistLine()+")";
      printError("no matching method call", str);
      return true;
    }
    return false;
  }
  else if(type=="print") {
    //just like semi, nothing this node can cause an error for
    Node* n=left;
    if(n->type=="empty") {
      return false;
    }
    n=n->getLeft();
    while(n) {
      if(n->typeCheckExp(table)) {
        string str="print("+left->getArglistLine()+");";
        printError("invalid expression in ", str);
        return true;
      }
      SymbolTable* tmp=n->evalExpResult(table);
      if(!(tmp)) {
      printError("invalid expression", right->getExpLine());
        return true;
      }
      if(tmp->id!="int") {
        string str="print("+left->getArglistLine()+");";
        printError("mismatching argument type", str);
        return true;
      }
      if(n->getNext()) {
        n=n->getNext();
      }
      else {
        break;
      }
    }
    return false;
  }
  else if(type=="cndtlstmt") {
    SymbolTable* tmp=left->getLeft()->evalExpResult(table);
    if(left->type=="if") {
      if(!(tmp)) {
      //if(left->typeCheckExp(table)) {
        string str="if("+left->getLeft()->getExpLine()+")";
        printError("invalid expression", str);
        return true;
      }
      return left->getRight()->typeCheckStmt(table);
    }
    //SymbolTable* tmp=left->getLeft()->evalExpResult(table);
    if(tmp->id!="int") {
      printError("invalid type for if statement", left->getExpLine());
      return true;
    }
    //tmp=left->getLeft()->evalExpResult(table);
    if(!(tmp)) {
      string str="if("+left->getLeft()->getExpLine()+")";
      printError("invalid expression", str);
      return true;
    }
    if(left->getRight()->typeCheckStmt(table)) {
      return true;
    }
    return left->getRight()->typeCheckStmt(table);
  }
  else if(type=="while") {
    //not worried about the result or type, just as long as it doesnt throw any
    //error, we can give it whatever exp. which makes sense from a c++ sense
    //anything thats not zero is true, but java must use booleans, and decaf
    //doesnt have any so Im just rolling with this for now
    SymbolTable* tmp=left->evalExpResult(table);
    if(!(tmp)) {
      string str="while("+left->getExpLine()+")";
      printError("invalid expression", str);
      return true;
    }
    if(right->typeCheckStmt(table)) {
      return true;
    }
    return false;
  }
  else if(type=="return") {
    if(constructor) {
      string str="return";
      if(left->type!="empty") {
        str+=left->getExpLine();
      }
      str+=";";
      printError("unexpected return statement", str);
      return true;
    }
    //check if void type, and returns nothing;
    if((left->name=="empty")&&(table->returnType->id=="void")) {
      return false;
    }
    else if(left->name!="empty") {
      SymbolTable* tmp=left->getLeft()->evalExpResult(table);
      //failed to find result of exp
      if(!(tmp)) {
        printError("invalid expression", left->left->getExpLine());
        return true;
      }
      if(table->returnType->id==tmp->id) {
        return (false||err);
      }
    }
    printError(
      "return type does not match method return type",
      left->getLeft()->getExpLine()
    );
    error=true;
    return true;
  }
  else if(type=="block") {
    left->block=block;//table->lookup(left->name);//just incase it wasnt already
    return left->typeCheck();
  }
  return err;
}
bool Node::typeCheckName(SymbolTable* table) {
  if(type=="dot") {
    SymbolTable* t=left->evalNameTable(table);
    if(!(t)) {
      return true;
    }
    t=t->lookup(right->name);
    if(!(t)) {
      printError("no matching member "+right->name, this->getNameLine());
      return true;
    }
  }
  else if(type=="array_access") {
    return !(table->lookup(left->name)->declared);
  }
  else if(name=="this") {
    //a obj.this case is handled in parse, so "this" is always fine now
    return false;
  }
  return !(table->lookup(name)->declared);
}
bool Node::typeCheckExp(SymbolTable* table) {
  err=false;
  SymbolTable* t;
  if(type=="name") {
    return left->typeCheckName(table);
  }
  else if(type=="function_call") {
    if(left->getRight()) {
      t=left->evalNameTable(table);
      if(!(t)) {
        return true;
      }
    }
    else {
      t=table;
    }
    string tmp=left->name+"#method";
    tmp+=right->argListTypes(table);
    t=t->lookup(tmp);
    return !(t->declared);
  }
  else if(type=="new_expr") {
    return left->newExprCheck(table);
  }
  else if(type=="decrement") {
    t=left->evalExpResult(table);
    if(!(t)) {
      return true;
    }
    return (t->id!="int");
  }
  else if(type=="increment") {
    t=left->evalExpResult(table);
    if(!(t)) {
      return true;
    }
    return (t->id!="int");
  }
  else if(type=="not") {
    t=left->evalExpResult(table);
    if(!(t)) {
      return true;
    }
    return (t->id!="int");
  }
  else if((type=="eq")||(type=="neq")) {
    string type1="";
    string type2="_";
    t=left->evalExpResult(table);
    if(!(t)) {
      return true;
    }
    type1=t->id;
    if(type1=="void") {
      string str=left->getExpLine()+name+right->getExpLine();
      printError("non comparable types, void", name);
      return true;
    }
    t=right->evalExpResult(table);
    if(!(t)) {
      return true;
    }
    type2=t->id;
    if(type2=="void") {
      string str=left->getExpLine()+name+right->getExpLine();
      printError("non comparable types, void", name);
      return true;
    }
    if((type1=="null")||(type2=="null")) {
      if((type1!="int")&&(type2!="int")) {
        return false;
      }
      string str=left->getExpLine()+name+right->getExpLine();
      printError("non comparable types", name);
      return true;
    }
    return (type1!=type2);
  }
  else if(type=="enclosed_exp") {
    return left->typeCheckExp(table);
  }
  else if((right)&&(left)) {
    if(left->typeCheck())
    t=left->evalExpResult(table);
    if(t->id!="int") {
      string str=left->getExpLine()+name+right->getExpLine();
      printError("Invalid type for operator "+name, str);
      return true;
    }
    t=left->evalExpResult(table);
    if(t->id!="int") {
      string str=left->getExpLine()+name+right->getExpLine();
      printError("Invalid type for operator "+name, str);
      return true;
    }
  }
  //rest are things that themselves cannot be type checked, ie inttype
  return false;
}
SymbolTable* Node::evalExpResult(SymbolTable* table) {
  SymbolTable* t;
  if(type=="name") {
    t=table->lookup(left->evalNameID(table));
    if(!t->declared) {
      printError("unexpected type "+t->id, getExpLine());
      return nullptr;
    }
    return t;
  }
  else if(type=="inttype") {
    return typeTable->lookup("int");
  }
  else if(type=="null") {
    return typeTable->lookup("null");
  }
  else if(name=="read") {
    return typeTable->lookup("int");
  }
  else if(type=="function_call") {
    if(left->getLeft()->getRight()) {
      t=left->evalNameTable(table);
      if(!(t)) {
        return nullptr;
      }
    }
    else {
      t=table;
    }
    string tmp=left->name+"#method";
    tmp+=right->argListTypes(table);
    t=t->lookup(tmp);
    return t;
  }
  else if(type=="new_expr") {
    return left->evalNewExpr();
  }
  else if(type=="enclosed_exp") {
    return left->evalExpResult(table);
  }
  if(!(right)) {
    t=left->evalExpResult(table);
    if(!(t)) {
      return nullptr;
    }
    if((t->id!="int")||(t->id=="void")) {
      string str=name+left->getExpLine();
      printError("invalid type for operator "+name, str);
      return nullptr;
    }
    return t;
  }
  if((type=="eq")||(type=="neq")) {
    t=left->evalExpResult(table);
    if(!(t)) {
      return nullptr;
    }
    if(t->id=="void") {
      string str=left->getExpLine()+name+right->getExpLine();
      printError("invalid type void for operator "+name, str);
      return nullptr;
    }
    SymbolTable* t2=right->evalExpResult(table);
    if(!(t2)) {
      return nullptr;
    }
    if(t2->id=="void") {
      string str=right->getExpLine()+name+right->getExpLine();
      printError("invalid type void for operator "+name, str);
      return nullptr;
    }
    if(t2!=t) {
      if(
        !(
          ((t2->id=="null")&&(t->id!="int"))
          ||((t2->id!="int")&&(t->id=="null"))
        )
      ) {
        string str=right->getExpLine()+name+right->getExpLine();
        printError("non comparable types", str);
        return nullptr;
      }
    }
    //arithmetic operators always return type int, even if compared types are objs
    return typeTable->lookup("int");
  }
  //everything left is an arithmetic operators
  t=left->evalExpResult(table);
  if(!(t)) {
    return nullptr;
  }
  if((t->id!="int")||(t->id=="void")) {
    string str=left->getExpLine()+name+right->getExpLine();
    printError("invalid type"+t->id+" for operator "+name, str);
    return nullptr;
  }
  t=right->evalExpResult(table);
  if(!(t)) {
    return nullptr;
  }
  if((t->id!="int")||(t->id=="void")) {
    string str=left->getExpLine()+name+right->getExpLine();
    printError("invalid type for operator "+name, str);
    return nullptr;
  }
  return t;
}
bool Node::newExprCheck(SymbolTable* table) {
  string str="";
  SymbolTable* t;
  if(type=="new_obj") {
    str+=left->name;
    t=typeTable->lookup(str);
    if((!(t))|(!t->declared)) {
      str="new "+str+"("+right->getArglistLine()+")";
      printError("undeclared type", str);
      return true;
    }
    name+="#const"+left->argListTypes(table);
    if(t->lookup(name)) {
      return false;
    }
    str="new"+str+"("+right->getArglistLine()+")";
    printError("no constructor matching", str);
    return true;
  }
  else if(type=="new_int") {
    //a "new int" is always possible
    return false;
  }
  else if(type=="new_int_array") {
    Node* n=left;
    while(n) {
      if(n->getLeft()->typeCheckExp(table)) {
        return true;
      }
      if(n->getNext()) {
        n=n->getNext();
      }
    }
    return false;
  }
  else if(type=="new_obj_array_exp") {
    str+=left->name;
    t=typeTable->lookup(str);
    if((!(t))||(!t->declared)) {
      str="new "+str+right->getBracketexpsLine()+right->getMultibracketsLine();
      printError("undeclared type", str);
      return true;
    }
    Node* n=right;
    while(n) {
      if(n->getLeft()->typeCheckExp(table)) {
        return true;
      }
      if(n->getNext()) {
        n=n->getNext();
      }
    }
    return false;
  }
  else if(type=="new_int_array_exp_brackets") {
    Node* n=left;
    while(n) {
      if(n->getLeft()->typeCheckExp(table)) {
        return true;
      }
      if(n->getNext()) {
        n=n->getNext();
      }
    }
    //there is no type checking for quantity of dimensions, and I could have
    //added arrays to the typetable, but seemed unnecessary at the time
    //when I made it because I thought it wouldnt have worked this way
    //and Im worried I wont be able to implement in this particular fashion
    //but was leading to using a 'when trying to acces said array, does it
    //have x amount of dims? okay or not okay'
    return false;
  }
  else if(type=="new_obj_array_exp_brackets") {
    str+=left->name;
    t=typeTable->lookup(str);
    if((!(t))||(!t->declared)) {
      str="new "+str+mid->getBracketexpsLine()+right->getMultibracketsLine();
      printError("undeclared type", str);
      return true;
    }
    Node* n=mid;
    while(n) {
      if(n->getLeft()->typeCheckExp(table)) {
        return true;
      }
      if(n->getNext()) {
        n=n->getNext();
      }
    }
    return false;
  }
  return false;
}
string Node::evalNameID(SymbolTable* table) {
  if(type=="this") {
    return table->id;
  }
  else if(type=="dot") {
    //this.obj.a
    SymbolTable* tmp=left->evalNameTable(table);
    if(!(tmp)) {
      return "";
    }
    tmp=tmp->lookup(name);
    if(!tmp) {
      string str=tmp->id+"."+name;
      string str2="no known member"+name;
      printError(str2, str);
      return "";
    }
    return tmp->id;
  }
  else if(type=="array_access") {
    return left->evalNameID(table);
  }
  SymbolTable* t=table->lookup(name);
  if(!t->declared) {
    printError("unexpected type "+t->id, getNameLine());
  }
  return t->id;
}
SymbolTable* Node::evalNameTable(SymbolTable* table) {
  if(name=="this") {
    SymbolTable* t=table;
    while(t->parent) {
      t=t->parent;
    }
    t=typeTable->lookup(t->id.substr(0, t->id.find("#", 0)));
    return t;
  }
  else if(type=="dot") {
    //this.obj.a
    SymbolTable* t=left->evalNameTable(table);
    if(!(t)) {
      return nullptr;
    }
    t=t->lookup(name);
    if(!(t)) {
      string str=t->id+"."+name;
      string str2="no known member"+name;
      printError(str2, str);
      return nullptr;
    }
  }
  else if(type=="array_access") {
    return left->evalNameTable(table);
  }
  SymbolTable* t=table->lookup(name);
  if(!t->declared) {
    return nullptr;
  }
  return t;
}
string Node::argListTypes(SymbolTable* table) {
  string str="";
  if(type=="empty") {
    return str;
  }
  else if(type=="arglst") {
    return left->argListTypes(table);
  }
  if(next) {
    str+="_";
    str+=evalExpResult(table)->id;
    str+=next->argListTypes(table);
    return str;
  }
  return "_"+evalExpResult(table)->id;
}
string Node::getNameLine() {
  string str="";
  if(type=="dot") {
    str+=left->getNameLine();
    str+="."+right->name;
  }
  else if(type=="array_access") {
    str+=left->getNameLine();
    str+="["+right->getExpLine()+"]";
  }
  else {
    str+=name;
  }
  return str;
}
string Node::getExpLine() {
  string str="";
  if(type=="name") {
    str+=left->getNameLine();
  }
  else if(type=="inttype") {
    str+=to_string(left->getInt());
  }
  else if(type=="null") {
    str+="null";
  }
  else if(type=="read") {
    str+="read()";
  }
  else if(type=="function_call") {
    str+=left->getNameLine();
    str+="("+right->getArglistLine()+")";
  }
  else if(type=="new_expr") {
    str+=left->getNewexprLine();
  }
  else if(type=="decrement") {
    str+="-"+left->getExpLine();
  }
  else if(type=="increment") {
    str+="+"+left->getExpLine();
  }
  else if(type=="not") {
    str+="!"+left->getExpLine();
  }
  else if(type=="eq") {
    str+=left->getExpLine();
    str+="==";
    str+=right->getExpLine();
  }
  else if(type=="neq") {
    str+=left->getExpLine();
    str+="!=";
    str+=right->getExpLine();
  }
  else if(type=="leq") {
    str+=left->getExpLine();
    str+="<=";
    str+=right->getExpLine();
  }
  else if(type=="geq") {
    str+=left->getExpLine();
    str+=">=";
    str+=right->getExpLine();
  }
  else if(type=="lt") {
    str+=left->getExpLine();
    str+="<";
    str+=right->getExpLine();
  }
  else if(type=="gt") {
    str+=left->getExpLine();
    str+=">";
    str+=right->getExpLine();
  }
  else if(type=="and") {
    str+=left->getExpLine();
    str+="&&";
    str+=right->getExpLine();
  }
  else if(type=="or") {
    str+=left->getExpLine();
    str+="||";
    str+=right->getExpLine();
  }
  else if(type=="plus") {
    str+=left->getExpLine();
    str+="+";
    str+=right->getExpLine();
  }
  else if(type=="minus") {
    str+=left->getExpLine();
    str+="-";
    str+=right->getExpLine();
  }
  else if(type=="times") {
    str+=left->getExpLine();
    str+="*";
    str+=right->getExpLine();
  }
  else if(type=="div") {
    str+=left->getExpLine();
    str+="/";
    str+=right->getExpLine();
  }
  else if(type=="mod") {
    str+=left->getExpLine();
    str+="%";
    str+=right->getExpLine();
  }
  else if(type=="enclosed_exp") {
    str+="(";
    str+=left->getExpLine();
    str+=")";
  }
  else {
    printError("Unknown error", str);
    error=true;
  }
  return str;
}
string Node::getArglistLine() {
  str="";
  if(type=="empty") {
    return str;
  }
  if(left) {
    str+=left->getArglistLine();
  }
  else {
    str+=name;
    if(next) {
      str+=", "+next->getArglistLine();
    }
  }
  return str;
}
SymbolTable* Node::evalNewExpr() {
  string str="";
  /*
  if(type=="new_obj") {
    str=left->name;
  }
  else if(type=="new_int") {
    str="int";
  }
  else if(type=="new_int_array") {
    str="int"+right->getMultibracketsLine();
  }
  else if(expr->type=="new_obj_array_exp") {
    str=left->name+right->getMultibracketsLine();
  }
  else if(expr->type=="new_int_array_exp_brackets") {
    str="int"+left->getMultibracketsLine()+right->getMultibracketsLine();
  }
  else if(expr->type=="new_obj_array_exp_brackets") {
    str=left->name+left->getMultibracketsLine()+right->getMultibracketsLine();
  }*/
  if(type=="new_obj") {
    str=left->name;
  }
  else if(type=="new_int") {
    str="int";
  }
  else if(type=="new_int_array") {
    str="int";
  }
  else if(type=="new_obj_array_exp") {
    str=left->name;
  }
  else if(type=="new_int_array_exp_brackets") {
    str="int";
  }
  else if(type=="new_obj_array_exp_brackets") {
    str=left->name;
  }
  SymbolTable* t=typeTable->lookup(str);
  if(!t->declared) {
    printError("unexpected type"+t->id, getNewexprLine());
    return nullptr;
  }
  return t;
}
string Node::getNewexprLine() {
  string str="";
  if(type=="new_obj") {
    str+="new "+left->name+"(";
    str+=right->getArglistLine()+")";
  }
  else if(type=="new_int") {
    str+="new int";
  }
  else if(type=="new_int_array") {
    str+="new int";
    str+=left->getBracketexpsLine();
  }
  else if(type=="new_obj_array_exp") {
    str+="new "+left->name;
    str+=right->getBracketexpsLine();
  }
  else if(type=="new_int_array_exp_brackets") {
    str+="new int";
    str+=left->getBracketexpsLine();
    str+=right->getMultibracketsLine();
  }
  else if(type=="new_obj_array_exp_brackets") {
    str+="new ";
    str+=left->name;
    str+=mid->getBracketexpsLine();
    str+=right->getMultibracketsLine();

  }
  return str;
}
string Node::getBracketexpsLine() {
  string str="[";
  str+=left->getExpLine();
  str+="]";
  if(next) {
    str+=next->getBracketexpsLine();
  }
  return str;
}
string Node::getMultibracketsLine() {
  string str="[]";
  if(next) {
    str+=next->getMultibracketsLine();
  }
  return str;
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
