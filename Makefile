CXX=g++
CXXFLAGS=-std=c++11 -ggdb -Wall -Wno-sign-compare
YACC=bison
YFLAGS=--report=state -W -d
LEX=flex++
LFLAGS=--warn
SRC=program5.cpp program5.hpp program5.tab.cpp program5_lex.cpp nodes.cpp Type.cpp Entry.cpp SymbolTable.cpp
HDRS=program5.tab.hpp nodes.hpp Type.hpp Entry.hpp SymbolTable.hpp

program5: $(SRC) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRC) -o program5

program5.tab.cpp: program5.ypp nodes.hpp Entry.hpp Type.hpp SymbolTable.hpp
	$(YACC) $(YFLAGS) program5.ypp

program5_lex.cpp: program5.lpp nodes.hpp Entry.hpp Type.hpp SymbolTable.hpp
	$(LEX) $(LFLAGS) program5.lpp

clean: 
	rm -f core.* program5.tab.* program5_lex.cpp program5 program5.output

