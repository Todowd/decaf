#include "Type.hpp"

Type::Type() {
  rtn=nullptr;
}

Type::Type(Entry *r) {
  rtn=r;
}

void Type::addParam(Entry* e) {
  params.push_back(e);
  return;
}

int Type::getParamCount() {
  return params.size();
}

Entry* Type::getParam(int n) {
  return params[n];
}

bool Type::equals(Type* t) {
  if((rtn->equals(t->rtn))&&(getParamCount()==t->getParamCount())) {
    for(int i=0; i<t->getParamCount(); i++) {
      if(!(params[i]->equals(t->getParam(i)))) {
        return false;
      }
    }
    return true;
  }
  return false;
}
