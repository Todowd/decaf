#ifndef ENTRY_H
#define ENTRY_H

#include<string>
using std::string;

class Entry {
  public:
    Entry();
    Entry(string i, string t, int dims=0);
    string id;
    string type;
    int dims;
    bool equals(Entry* e);
};

#endif
