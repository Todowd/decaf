#ifndef TYPE_H
#define TYPE_H

#include<vector>
using std::vector;

#include "Entry.hpp"

class Type {
  public:
    Type();
    Type(Entry* r);
    Entry* rtn;
    void addParam(Entry* e);
    Entry* getParam(int n);
    int getParamCount();
    bool equals(Type* t);
  private:
    vector<Entry*> params;
};

#endif
