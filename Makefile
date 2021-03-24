CPPFLAGS += -std=c++17 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include


bin/c_compiler : src/compiler.o src/parser.tab.o src/lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^

bin/test_compiler: src/compiler.cpp
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/test_compiler $^
src/lexer.yy.cpp: src/lexer.l src/parser.tab.hpp
	flex -o src/lexer.yy.cpp ./src/lexer.l

src/parser.tab.cpp src/parser.tab.hpp: src/parser.y
	bison -v -d -t src/parser.y -o src/parser.tab.cpp

%.o: %.cpp
	g++ $(CPPFLAGS)  -c -o $@ $^

clean :
	rm -f src/*.o
	rm -f bin/*
	rm -f src/*.tab.*
	rm -f src/*.yy.cpp
	rm -f src/*.output
	rm -f include/*.gch
	rm -f include/ast/*.gch
	rm -f output/*
