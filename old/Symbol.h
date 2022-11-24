#ifndef SYMBOL_H
#define SYMBOL_H

class Symbol {
  public:
    Symbol();
    Symbol(string i, SymbolTable* t);
    string id;
    SymbolTable* type;
    int getDims();
    int getDimSize(int i);
    void setDimSize(int i, int v);
    private
    vector<Symbol*> dimSize;
};

class Integer:public Symbol {
  public:
    Integer(string id);
    void setValue(int v);
    int getValue();
  private:
    int val;
};

class Constructor:public Symbol {
  public:
    Constructor(string id, vector<Symbol> p);
    Symbol getParam(int i);
    int getParamCount();
  private:
    vector<Symbol> params;
};

class Method:public Symbol {
  public:
    Method(string id, vector<Symbol> p, SymbolTable* t);
    ~Method();
    Symbol getParam(int i);
    int getParamCount();
    SymbolTable* returnType;
  private:
    vector<Symbol> params;
};

#endif
