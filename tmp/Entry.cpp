#include "Entry.hpp"

Entry::Entry() {
  id=".";//because not valid id
  type=".";
}

Entry::Entry(string i, string t, int dim) {
  id=i;
  dims=dim;
  type=t;
}

bool Entry::equals(Entry* e) {
  return ((dims==e->dims)&&(type==e->type)&&(id==e->id));
}
