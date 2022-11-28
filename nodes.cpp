/*
nodes.cpp
COSC4785
Tyler O'Dowd
10/24/22

Nodes that make up the syntax tree
*/
#include<iostream>
using std::cout;
using std::endl;

#include "nodes.hpp"

extern int ind;

Node::Node(Node* Left, Node* Right, Node* Mid, Node* Last):
i(0), left(Left), right(Right), mid(Mid), last(Last) {
  next=nullptr;
  err=false;
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
