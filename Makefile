CXX=g++
CXXFLAGS=-std=c++11 -ggdb -Wall -Wno-sign-compare
YACC=bison
YFLAGS=--report=state -W -d
LEX=flex++
LFLAGS=--warn
SRC=program6.cpp program6.hpp program6.tab.cpp program6_lex.cpp nodes.cpp Type.cpp Entry.cpp SymbolTable.cpp
HDRS=program6.tab.hpp nodes.hpp Type.hpp Entry.hpp SymbolTable.hpp

program6: $(SRC) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRC) -o program6

program6.tab.cpp: program6.ypp nodes.hpp Entry.hpp Type.hpp SymbolTable.hpp
	$(YACC) $(YFLAGS) program6.ypp

program6_lex.cpp: program6.lpp nodes.hpp Entry.hpp Type.hpp SymbolTable.hpp
	$(LEX) $(LFLAGS) program6.lpp

clean: 
	rm -f core.* program6.tab.* program6_lex.cpp program6 program6.output

