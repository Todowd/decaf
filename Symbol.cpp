#include "Symbol.hpp"

Symbol::Symbol() {
  id=".";//because not valid id
}

Symbol::Symbol(string i, string t, int dim) {
  id=i;
  dims=dim;
  type=t;
}
