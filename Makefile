CXX=g++
CXXFLAGS=-ggdb -Wall -Wno-sign-compare
YACC=bison
YFLAGS=--report=state -Wcounterexamples -d
LEX=flex++
LFLAGS=--warn
SRC=program5.cpp program5.hpp program5.tab.cpp program5_lex.cpp nodes.cpp
HDRS=program5.tab.hpp nodes.hpp

program5: $(SRC) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRC) -o program5

program5.tab.cpp: program5.ypp nodes.hpp
	$(YACC) $(YFLAGS) program5.ypp

program5_lex.cpp: program5.lpp nodes.hpp
	$(LEX) $(LFLAGS) program5.lpp

clean: 
	rm -f core.* program5.tab.* program5_lex.cpp program5

