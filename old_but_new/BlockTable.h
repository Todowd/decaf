#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include<unordered_map>
using std::unordered_map;

#include<vector>
using std::vector;

class BlockTable {
  public:
    BlockTable();
    BlockTable(Symbol i, BlockTable* p=nullptr);

    static MasterTable* classes;

    BlockTable* parent;

    int insert(Symbol s);
    int insert(Integer s);
    Symbol* lookup(Symbol s);//returns the symbol of the method, construct, or
    Symbol* lookup(Integer s);

  private:
    void print(int& indent);
    unordered_map<string, Symbol> vars;
    unordered_map<string, Integer> varInts;//Im avoiding using typeid, & typeof
      //but yes couldve done it with the inheretence that I originally setup
    vector<BlockTable*> blocks;
};

#endif
